#ifndef AirConState_h
#define AirConState_h

#include <Arduino.h>
#include <HttpEndpoint.h> // for JsonObject

#include <DebugConsole.h>

#define DEFAULT_power false
#define DEFAULT_MODE "auto"
#define DEFAULT_TEMPERATURE 24.0f
#define DEFAULT_FLOWSPEED 3
#define DEFAULT_VERTICALSWING false

class AirConState {
 public:
  bool power;
  String mode;
  float temperature;
  int flowspeed;
  bool verticalswing;
  float sensor_temp_inside;
  float sensor_temp_outside;
  byte registers[0x25][4];

  static void read(AirConState& settings, JsonObject& root) {
      //printf("root size1: %d\n", root.memoryUsage());
    root["power"] = settings.power;
    root["mode"] = settings.mode;
    root["temperature"] = settings.temperature;
    root["flowspeed"] = getFlowSpeedString(settings.flowspeed);
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

  static const char *getFlowSpeedString(int flowspeed) {
    switch (flowspeed) {
      case 0x03: return "low";
      case 0x04: return "gentle";
      case 0x05: return "medium";
      case 0x06: return "strong";
      case 0x07: return "high";
      case 0x11: return "auto";
      case 0x12: return "quiet";
      default: return "unknown";
    }
  }

static byte flowSpeedFromString(String flowspeed) {
    if (flowspeed.equals("low")) return 0x03;
    if (flowspeed.equals("gentle")) return 0x04;
    if (flowspeed.equals("medium")) return 0x05;
    if (flowspeed.equals("strong")) return 0x06;
    if (flowspeed.equals("high")) return 0x07;
    if (flowspeed.equals("auto")) return 0x11;
    if (flowspeed.equals("quiet")) return 0x12;
    return 0x00;
}


  static void haRead(AirConState& settings, JsonObject& root) {
    //root["power"] = settings.power ? "on" : "off";
    root["mode"] = settings.power ? settings.mode : "off";
    root["temperature"] = settings.temperature;
    root["flowspeed"] = getFlowSpeedString(settings.flowspeed);
    root["verticalswing"] = settings.verticalswing ? "on" : "off";
    root["sensor_temp_inside"] = settings.sensor_temp_inside;
    root["sensor_temp_outside"] = settings.sensor_temp_outside;
  }

  static StateUpdateResult haUpdate(JsonObject& root, AirConState& airConState) {
    //D.printf("haUpdate\n");
    D.printf("haUpdate power: %d ; temperature: %f\n", root["power"].as<bool>(), root["temperature"].as<float>());

    String newpowerStr = root.containsKey("power") ? root["power"].as<String>() : (airConState.power ? "on" : "off");
    String newMode = root.containsKey("mode") ? root["mode"].as<String>() : airConState.mode;
    if (newMode.equalsIgnoreCase("off")) {
        newMode=airConState.mode;
        newpowerStr="off";
    } else {
        newpowerStr="on";
    }
    bool newpower = newpowerStr.equalsIgnoreCase("on") || newpowerStr.equalsIgnoreCase("true");

    float newTemperature = root.containsKey("temperature") ? root["temperature"].as<float>() : airConState.temperature;
    int newFlowspeed = root.containsKey("flowspeed") ? flowSpeedFromString(root["flowspeed"].as<String>()) : airConState.flowspeed;
    bool newVerticalswing = root.containsKey("verticalswing") ? root["verticalswing"].as<bool>() : airConState.verticalswing;
    bool changed = false;

    if (airConState.power != newpower) {
      airConState.power = newpower;
      changed=true;
    }
    if (!airConState.mode.equals(newMode)) {
      if (!newMode.equals("cool") && !newMode.equals("heat") && !newMode.equals("dry") && !newMode.equals("fan_only") && !newMode.equals("auto"))
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
