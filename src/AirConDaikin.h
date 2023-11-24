#ifndef AirConDaikin_h
#define AirConDaikin_h

#include "AirConDevice.h"
#include "AirConStateService.h"

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

    inline constexpr static byte REGISTERSLOOPASK[] = {
        MODE,
        FLOWAIR_DIRECTION,
        MODE_POWER,
        TEMP_SENSORS
    };
};

class MODE {
  public:
    static const byte AUTO                = 0x01;
    static const byte DRY                 = 0x02;
    static const byte COOL                = 0x03;
    static const byte HEAT                = 0x04;
    static const byte FAN_ONLY            = 0x06;
};

class MODE_POWER {
  public:
    static const byte NORMALPOWER         = 0x00;
    static const byte MAXPOWER            = 0x02;
};

class VERTICALSWING {
  public:
    static const byte OFF                 = 0x00;
    static const byte ON                  = 0x01;
};

class HORIZONTALSWING {
  public:
    static const byte OFF                 = 0x00;
    static const byte ON                  = 0x01;
};

class AirConDaikin : AirConDevice {
  public:
    AirConDaikin(AirConStateService *stateService);
    ~AirConDaikin();

    void setState(AirConState *newStatus);
    AirConState *getState();
    void loop();

  private:
    AirConStateService *stateService;
    update_handler_id_t myUpdateHandler;
    HardwareSerial *serial;
    int askStateIndex;
    byte outBuffer[16], inBuffer[16];
    int inBufferIndex;
    uint32_t latestmsg;
    byte registers[0x25][4];

    void loopAskState();
    void initSerial();
    void readSerial();
    void decodeInputMessage();
    void decodeRegisterAnswer(byte *msg, int len);
    void decodeRegister2Answer(byte *msg, int len);
    void decodeRegisterMode();
    void decodeRegisterFlowairDirection();
    void decodeRegisterTempSensors();
    void sendMessage(byte packet[], int length);
    void sendMode(bool onoff, String mode, float temp, int flowspeed);
    void sendSwing(bool vertical, bool horizontal);
};

#endif
