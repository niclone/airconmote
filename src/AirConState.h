#ifndef AirConState_h
#define AirConState_h

#include <Arduino.h>
#include <HttpEndpoint.h> // for JsonObject

#include <DebugConsole.h>

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
  byte registers[0x25][4];

  static void read(AirConState& settings, JsonObject& root) {
      //printf("root size1: %d\n", root.memoryUsage());
    root["onoff"] = settings.onoff;
    root["mode"] = settings.mode;
    root["temperature"] = settings.temperature;
    root["flowspeed"] = settings.flowspeed;
    root["verticalswing"] = settings.verticalswing;
    root["sensor_temp_inside"] = settings.sensor_temp_inside;
    root["sensor_temp_outside"] = settings.sensor_temp_outside;

#ifdef DEBUGREGISTERS
    JsonArray jregisters = root.createNestedArray("registers");
      //printf("root size2: %d\n", root.memoryUsage());
    for (int i=0; i<0x25; i++) {
        JsonArray nested = jregisters.createNestedArray();
        for (int j=0; j<4; j++) {
            nested.add(settings.registers[i][j]);
        }
    }
      //printf("root size3: %d\n", root.memoryUsage());
#endif
  }

  static StateUpdateResult update(JsonObject& root, AirConState& airConState) {
    return haUpdate(root, airConState);
  }

  static void haRead(AirConState& settings, JsonObject& root) {
    return read(settings, root);
  }

  static StateUpdateResult haUpdate(JsonObject& root, AirConState& airConState) {
    //D.printf("haUpdate\n");
    D.printf("haUpdate onoff: %d ; temperature: %f\n", root["onoff"].as<bool>(), root["temperature"].as<float>());

    bool newOnOff = root.containsKey("onoff") ? root["onoff"].as<bool>() : airConState.onoff;
    String newMode = root.containsKey("mode") ? root["mode"].as<String>() : airConState.mode;
    float newTemperature = root.containsKey("temperature") ? root["temperature"].as<float>() : airConState.temperature;
    float newFlowspeed = root.containsKey("flowspeed") ? root["flowspeed"].as<int>() : airConState.flowspeed;
    bool newVerticalswing = root.containsKey("verticalswing") ? root["verticalswing"].as<bool>() : airConState.verticalswing;
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
