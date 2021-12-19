#ifndef AirConDevice_h
#define AirConDevice_h

#include <Arduino.h>
#include <AirConState.h>

/**
 * @brief common class for an aircon device
 */
class AirConDevice {
  public:
    virtual void setState(AirConState *newState);
    virtual AirConState *getState();
    virtual void loop();
};






#endif
