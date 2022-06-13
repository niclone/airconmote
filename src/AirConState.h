#ifndef AirConState_h
#define AirConState_h

#include <Arduino.h>
#include <HttpEndpoint.h> // for JsonObject

#define DEFAULT_ONOFF false
#define DEFAULT_MODE "auto"
#define DEFAULT_TEMPERATURE 24.0f
#define DEFAULT_FLOWSPEED 3
#define DEFAULT_VERTICALSWING false

class AirConState {
 public:
  bool onoff;
  String mode;
  float temperature;
  int flowspeed;
  bool verticalswing;
  float sensor_temp_inside;
  float sensor_temp_outside;
  byte registers[0x25][5];

  static void read(AirConState& settings, JsonObject& root) {
      //printf("root size1: %d\n", root.memoryUsage());
    root["onoff"] = settings.onoff;
    root["mode"] = settings.mode;
    root["temperature"] = settings.temperature;
    root["flowspeed"] = settings.flowspeed;
    root["verticalswing"] = settings.verticalswing;
    root["sensor_temp_inside"] = settings.sensor_temp_inside;
    root["sensor_temp_outside"] = settings.sensor_temp_outside;
    JsonArray jregisters = root.createNestedArray("registers");
      //printf("root size2: %d\n", root.memoryUsage());
    for (int i=0; i<0x25; i++) {
        JsonArray nested = jregisters.createNestedArray();
            /*
            printf("adding [%x] : %x %x %x %x %x\n",
                i,
                settings.registers[i][0],
                settings.registers[i][1],
                settings.registers[i][2],
                settings.registers[i][3],
                settings.registers[i][4]
            );
            */
        for (int j=0; j<5; j++) {
            nested.add(settings.registers[i][j]);
        }
    }
      //printf("root size3: %d\n", root.memoryUsage());
  }

  static StateUpdateResult update(JsonObject& root, AirConState& airConState) {
    return haUpdate(root, airConState);
  }

  static void haRead(AirConState& settings, JsonObject& root) {
    return read(settings, root);
  }

  static StateUpdateResult haUpdate(JsonObject& root, AirConState& airConState) {
    bool newOnOff = root["onoff"] | DEFAULT_ONOFF;
    String newMode = root["mode"] | DEFAULT_MODE;
    float newTemperature = root["temperature"] | DEFAULT_TEMPERATURE;
    float newFlowspeed = root["flowspeed"] | DEFAULT_FLOWSPEED;
    bool newVerticalswing = root["verticalswing"] | DEFAULT_ONOFF;
    bool changed = false;

    if (airConState.onoff != newOnOff) {
      airConState.onoff = newOnOff;
      changed=true;
    }
    if (!airConState.mode.equals(newMode)) {
      if (!newMode.equals("snow") && !newMode.equals("heat") && !newMode.equals("dry") && !newMode.equals("air"))
        return StateUpdateResult::ERROR;
      airConState.mode = newMode;
      changed=true;
    }
    if (airConState.temperature != newTemperature) {
      if (newTemperature < 10.0f || newTemperature > 30.0f)
        return StateUpdateResult::ERROR;
      airConState.temperature = newTemperature;
      changed=true;
    }
    if (airConState.flowspeed != newFlowspeed) {
      if (!((newFlowspeed >= 3 && newFlowspeed <= 7) || (newFlowspeed >= 0x11 && newFlowspeed <= 0x12)))
        return StateUpdateResult::ERROR;
      airConState.flowspeed = newFlowspeed;
      changed=true;
    }
    if (airConState.verticalswing != newVerticalswing) {
      airConState.verticalswing = newVerticalswing;
      changed=true;
    }
    return changed ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
  }
};

#endif
