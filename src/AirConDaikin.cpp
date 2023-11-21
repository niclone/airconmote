#include "AirConDaikin.h"

AirConDaikin::AirConDaikin(AirConStateService *stateService) {
    this->stateService = stateService;
    initSerial();
    latestAskedState=0;
    inBufferIndex=0;
    latestmsg=0;

    // register an update handler
    myUpdateHandler = stateService->addUpdateHandler(
        [&](const String& originId) {
            Serial.print("The aircon's state has been updated by: ");
            Serial.println(originId);
            if (originId != "device") {
                this->stateService->read([&](AirConState& state) {
                    setState(&state);
                });
            }
        }
    );
}

AirConDaikin::~AirConDaikin() {
    // remove the update handler
    stateService->removeUpdateHandler(myUpdateHandler);

    serial->end();
}

void AirConDaikin::setState(AirConState *newstate) {
    //Serial.println("setting state to device ...");
    sendMode(newstate->onoff, newstate->mode, newstate->temperature, newstate->flowspeed);
    sendSwing(newstate->verticalswing, false);
}

AirConState *AirConDaikin::getState() {
    return NULL;
}

void AirConDaikin::initSerial() {
    serial = &Serial1;
    serial->begin(2400, SERIAL_8N2, 16, 17);
}

void AirConDaikin::loop() {
    loopAskState();
    readSerial();
}

void AirConDaikin::loopAskState() {
    if (millis() - latestmsg > 500) {
        if (++latestAskedState > 0x24) latestAskedState=0x01;
        byte msg[]={MSGCODE::READ_REGISTER, (byte)latestAskedState};
        sendMessage(msg, sizeof(msg));
    }
}

void AirConDaikin::readSerial() {
    while(serial->available()) {
        byte b;
        size_t readed = serial->readBytes(&b, 1);
        //Serial.printf("got hex : %x\n", b);
        if (readed != 1) {
            // something bad happened !
            Serial.printf("read error: %d\n", readed);
            return;
        }
        switch (inBufferIndex) {
            case 0:
                if (b == 0x06) {
                    inBuffer[inBufferIndex++]=b;
                } else inBufferIndex = 0;
                break;

            case 1:
                if (b == 0x02) {
                    inBuffer[inBufferIndex++]=b;
                } else inBufferIndex = 0;
                break;

            default:
                if (inBufferIndex + 1 < sizeof(inBuffer)) {
                    inBuffer[inBufferIndex++]=b;
                    if (b == 0x03) {
                        decodeInputMessage();
                        inBufferIndex=0;
                    }
                } else {
                    // packet too big, reset !
                    inBufferIndex=0;
                }
                break;
        }
    }
}

void AirConDaikin::decodeInputMessage() {
    //Serial.printf("decodeInputMessage: %x %x (...)\n", inBuffer[0], inBuffer[1]);

    byte inMessage[sizeof(inBuffer-2)];
    for (int i=2; i<inBufferIndex; i++) {
        inMessage[i-2]=inBuffer[i] - 0x30;
    }

    int len = inBufferIndex-2;

    //Serial.printf("inMessage(%d): %x %x (...)\n", len, inMessage[0], inMessage[1]);

    switch(inMessage[0]) {
        case MSGCODE::REGISTER_ANSWER: decodeRegisterAnswer(&inMessage[1], len-1); break;
        case MSGCODE::REGISTER2_ANSWER: decodeRegister2Answer(&inMessage[1], len-1); break;
    }
}

void AirConDaikin::decodeRegisterAnswer(byte *inMessage, int len) {
    //Serial.printf("decodeRegisterAnswer: %x %x (...)", inMessage[0], inMessage[1]);
    // update local registers
    int registernum = inMessage[0];
    if (registernum > 0 && registernum < 0x25) {
        memcpy(&registers[registernum], &inMessage[1], 5);

            /*
            printf("register1 [%x] : %x %x %x %x %x\n",
                registernum,
                registers[registernum][0],
                registers[registernum][1],
                registers[registernum][2],
                registers[registernum][3],
                registers[registernum][4]
            );
            */


        stateService->update([&](AirConState& state) {

            bool changed = false;
            if (memcmp(&registers[registernum], &state.registers[registernum], 5) != 0) {
                memcpy(&state.registers[registernum], &registers[registernum], 5);
                changed = true;
            }
            return changed ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
        }, "device");

    }


    switch(inMessage[0]) {
        case REGISTER::MODE:
            decodeRegisterMode();
            break;
        case REGISTER::FLOWAIR_DIRECTION:
            decodeRegisterFlowairDirection();
            break;
        case REGISTER::MODE_POWER:
            break;
        case REGISTER::TEMP_SENSORS:
            decodeRegisterTempSensors();
            break;
    }
}

void AirConDaikin::decodeRegisterMode() {
    //Serial.println("decoding mode state of device ...");
    const byte *reg = registers[REGISTER::MODE];
    bool onoff = reg[0] == 0x01 ? true : false;
    String mode;
    switch(reg[1]) {
        case MODE::AUTO: mode = "auto"; break;
        case MODE::DRY:  mode = "dry";  break;
        case MODE::SNOW: mode = "snow"; break;
        case MODE::HEAT: mode = "heat"; break;
        case MODE::AIR:  mode = "air";  break;
    }
    float temperature = reg[2] / 2.0 + 10.0;
    int flowspeed = reg[3];

    stateService->update([&](AirConState& state) {
        bool changed = false;
        if (state.onoff != onoff) {
            state.onoff = onoff;
            changed=true;
        }
        if (state.mode != mode) {
            state.mode = mode;
            changed=true;
        }
        if (state.temperature != temperature) {
            state.temperature = temperature;
        }
        if (state.flowspeed != flowspeed) {
            state.flowspeed = flowspeed;
        }
        return changed ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
    }, "device");
}

void AirConDaikin::decodeRegisterFlowairDirection() {
    //Serial.println("decoding flowair state of device ...");
    const byte *reg = registers[REGISTER::FLOWAIR_DIRECTION];
    stateService->update([&](AirConState& state) {
        bool changed = false;
        bool verticalswing = state.verticalswing;
        switch(reg[1]) {
            case VERTICALSWING::OFF: verticalswing = false; break;
            case VERTICALSWING::ON:  verticalswing = true;  break;
        }
        if (verticalswing != state.verticalswing) {
            state.verticalswing = verticalswing;
            changed = true;
        }
        return changed ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
    }, "device");
}

void AirConDaikin::decodeRegisterTempSensors() {
    //Serial.println("decoding mode state of device ...");
    const byte *reg = registers[REGISTER::TEMP_SENSORS];
    float temp_inside = reg[0] / 2.0 - 40.0;
    float temp_outside = reg[1] / 2.0 - 40.0;

    stateService->update([&](AirConState& state) {
        bool changed = false;
        if (state.sensor_temp_inside != temp_inside) {
            state.sensor_temp_inside = temp_inside;
            changed=true;
        }
        if (state.sensor_temp_outside != temp_outside) {
            state.sensor_temp_outside = temp_outside;
            changed=true;
        }
        return changed ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
    }, "device");
}


void AirConDaikin::decodeRegister2Answer(byte *inMessage, int len) {
    //Serial.printf("decodeRegister2Answer: %x %x (...)", inMessage[0], inMessage[1]);
}

void AirConDaikin::sendMessage(byte message[], int length) {
    //Serial.printf("sendMessage: %x %x (...)\n", message[0], message[1]);
    outBuffer[0]=0x06;
    outBuffer[1]=0x02;
    byte checksum=0;
    for (int i=0; i<length; i++) {
        byte b=message[i] + 0x30;
        checksum+=b;
        outBuffer[2+i]=b;
    }
    outBuffer[2+length]=checksum;
    outBuffer[3+length]=0x03;

    serial->write(outBuffer, 4+length);
    latestmsg=millis();

    if (message[0] == MSGCODE::WRITE_REGISTER) {
        // update local registers
        int registernum = message[1];
        if (registernum > 0 && registernum < 0x25) {
            memcpy(&registers[registernum], &message[2], 5);
        }
    }
}

void AirConDaikin::sendMode(bool onoff, String mode, float temp, int flowspeed) {
    byte message[6]={0,0,0,0,0,0};
    message[0] = MSGCODE::WRITE_REGISTER;
    message[1] = REGISTER::MODE;

    // encode onoff :
    message[2] = onoff ? 0x01 : 0x00;

    // encode mode :
    if (mode.equals("auto")) {
        message[3]=MODE::AUTO; // auto
    } else if (mode.equals("dry")) {
        message[3]=MODE::DRY; // dry
    } else if (mode.equals("snow")) {
        message[3]=MODE::SNOW; // snow
    } else if (mode.equals("heat")) {
        message[3]=MODE::HEAT; // heat
    } else if (mode.equals("air")) {
        message[3]=MODE::AIR; // air
    } else {
        //console.error("setMode to unknown mode : ", mode);
        return;
    }

    // encode temp :
    message[4] = (temp - 10) * 2;

    // encode flowspeed :
    if (flowspeed >= 1 || flowspeed <= 7) {
        message[5] = flowspeed;
    } else {
        //console.error("setMode to unknown flowspeed : ", flowspeed);
        return;
    }

    sendMessage(message, sizeof(message));
}

void AirConDaikin::sendSwing(bool vertical, bool horizontal) {
    byte message[6]={0,0,0,0,0,0};
    message[0] = MSGCODE::WRITE_REGISTER;
    message[1] = REGISTER::FLOWAIR_DIRECTION;

    // encode vertical swing :
    message[2] = vertical ? VERTICALSWING::ON : VERTICALSWING::OFF;
    message[3] = vertical ? 15 : 0;

    message[4] = horizontal ? HORIZONTALSWING::ON : HORIZONTALSWING::OFF;
    message[5] = horizontal ? 15 : 0;

    sendMessage(message, sizeof(message));
}

bool isRegisterDiff(int idx, byte message[5]) {

}
