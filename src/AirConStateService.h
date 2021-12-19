#ifndef AirConStateService_h
#define AirConStateService_h

#include <AirConMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#define DEFAULT_ONOFF false
#define DEFAULT_MODE "auto"
#define DEFAULT_TEMPERATURE 24.0f
#define DEFAULT_FLOWSPEED 3

#define AIRCON_SETTINGS_ENDPOINT_PATH "/rest/airConState"
#define AIRCON_SETTINGS_SOCKET_PATH "/ws/airConState"

class AirConState {
 public:
  bool onoff;
  String mode;
  float temperature;
  int flowspeed;

  static void read(AirConState& settings, JsonObject& root) {
    root["onoff"] = settings.onoff;
    root["mode"] = settings.mode;
    root["temperature"] = settings.temperature;
    root["flowspeed"] = settings.flowspeed;
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
      if (newFlowspeed < 1 || newFlowspeed > 5)
        return StateUpdateResult::ERROR;
      airConState.flowspeed = newFlowspeed;
      changed=true;
    }
    return changed ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
  }
};

class AirConStateService : public StatefulService<AirConState> {
 public:
  AirConStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    AirConMqttSettingsService* airConMqttSettingsService);
  void begin();

 private:
  HttpEndpoint<AirConState> _httpEndpoint;
  MqttPubSub<AirConState> _mqttPubSub;
  WebSocketTxRx<AirConState> _webSocket;
  AsyncMqttClient* _mqttClient;
  AirConMqttSettingsService* _airConMqttSettingsService;

  void registerConfig();
  void onConfigUpdated();
};

#endif
