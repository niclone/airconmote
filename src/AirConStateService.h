#ifndef AirConStateService_h
#define AirConStateService_h

#include <AirConMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>

#define LED_PIN 2
#define PRINT_DELAY 5000

#define DEFAULT_MODE "auto"
#define DEFAULT_TEMPERATURE 24.0f
#define DEFAULT_FLOWSPEED 3

// Note that the built-in LED is on when the pin is low on most NodeMCU boards.
// This is because the anode is tied to VCC and the cathode to the GPIO 4 (Arduino pin 2).
#ifdef ESP32
#define LED_ON 0x1
#define LED_OFF 0x0
#elif defined(ESP8266)
#define LED_ON 0x0
#define LED_OFF 0x1
#endif

#define AIRCON_SETTINGS_ENDPOINT_PATH "/rest/airConState"
#define AIRCON_SETTINGS_SOCKET_PATH "/ws/airConState"

class AirConState {
 public:
  String mode;
  float temperature;
  int flowspeed;

  static void read(AirConState& settings, JsonObject& root) {
    root["mode"] = settings.mode;
    root["temperature"] = settings.temperature;
    root["flowspeed"] = settings.flowspeed;
  }

  static StateUpdateResult update(JsonObject& root, AirConState& airConState) {
    String newMode = root["mode"] | DEFAULT_MODE;
    float newTemperature = root["temperature"] | DEFAULT_TEMPERATURE;
    float newFlowspeed = root["flowspeed"] | DEFAULT_FLOWSPEED;
    bool changed = false;
    if (!airConState.mode.equals(newMode)) {
      airConState.mode = newMode;
      changed=true;
    }
    if (airConState.temperature != newTemperature) {
      airConState.temperature = newTemperature;
      changed=true;
    }
    if (airConState.flowspeed != newFlowspeed) {
      airConState.flowspeed = newFlowspeed;
      changed=true;
    }
    return changed ? StateUpdateResult::CHANGED : StateUpdateResult::UNCHANGED;
  }

  static void haRead(AirConState& settings, JsonObject& root) {
    root["mode"] = settings.mode;
    root["temperature"] = settings.temperature;
    root["flowspeed"] = settings.flowspeed;
  }

  static StateUpdateResult haUpdate(JsonObject& root, AirConState& airConState) {
    String newMode = root["mode"] | DEFAULT_MODE;
    float newTemperature = root["temperature"] | DEFAULT_TEMPERATURE;
    float newFlowspeed = root["flowspeed"] | DEFAULT_FLOWSPEED;
    bool changed = false;
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
