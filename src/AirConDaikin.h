#ifndef AirConDaikin_h
#define AirConDaikin_h

#include "AirConDevice.h"

class MSGCODE {
  public:
    static const byte WRITE_REGISTER      = 0x14;
    static const byte READ_REGISTER       = 0x16;
    static const byte READ_REGISTER2      = 0x22; // ?

    static const byte REGISTER_ANSWER     = 0x17;
    static const byte REGISTER2_ANSWER    = 0x23;
};

class REGISTER {
  public:
    static const byte MODE                = 0x01;
    static const byte FLOWAIR_DIRECTION   = 0x05;
    static const byte MODE_POWER          = 0x06;
    static const byte TEMP_SENSORS        = 0x09;
};

class MODE {
  public:
    static const byte AUTO                = 0x01;
    static const byte DRY                 = 0x02;
    static const byte SNOW                = 0x03;
    static const byte HEAT                = 0x04;
    static const byte AIR                 = 0x06;
};

class AirConDaikin : AirConDevice {
  public:
    AirConDaikin();
    ~AirConDaikin();

    void setState(AirConState *newStatus);
    AirConState *getState();
    void loop();

  private:
    HardwareSerial *serial;
    AirConState realState;
    int latestAskedState;
    byte outBuffer[16], inBuffer[16];
    int inBufferIndex;
    uint32_t latestmsg;

    void loopAskState();
    void initSerial();
    void readSerial();
    void decodeInputMessage();
    void decodeRegisterAnswer(byte *msg, int len);
    void decodeRegister2Answer(byte *msg, int len);
    void decodeRegisterAnswerMode(byte *inMessage, int len);
    void sendMessage(byte packet[], int length);
    void sendMode(bool onoff, String mode, float temp, int flowspeed);
};

#endif
