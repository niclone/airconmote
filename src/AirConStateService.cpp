#include <AirConStateService.h>

#include "AirConDaikin.h"
#include "DebugConsole.h"

AirConStateService::AirConStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager,
                                     AsyncMqttClient* mqttClient,
                                     AirConMqttSettingsService* airConMqttSettingsService) :
    _httpEndpoint(AirConState::read,
                  AirConState::update,
                  this,
                  server,
                  AIRCON_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED, 4096U),
    _mqttPubSub(AirConState::haRead, AirConState::haUpdate, this, mqttClient, "", "", 4096U),
    _webSocket(AirConState::read,
               AirConState::update,
               this,
               server,
               AIRCON_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED, 4096U),
    _mqttClient(mqttClient),
    _airConMqttSettingsService(airConMqttSettingsService) {

  aircondevice = (AirConDevice *)new AirConDaikin(this);

  // configure MQTT callback
  _mqttClient->onConnect(std::bind(&AirConStateService::registerConfig, this));

  // configure update handler for when the airCon settings change
  _airConMqttSettingsService->addUpdateHandler([&](const String& originId) { registerConfig(); }, false);

  // configure settings service update handler to update aircon state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(originId); }, false);
}

void AirConStateService::begin() {
  //onConfigUpdated();
}

void AirConStateService::loop() {
    aircondevice->loop();
}

void AirConStateService::onConfigUpdated(const String& originId) {
    D.printf("onConfigUpdated, originId: %s\n", originId.c_str());
  //digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
  //aircondevice->setState(&_state);
}

void AirConStateService::registerConfig() {
  if (!_mqttClient->connected()) {
    return;
  }
  String configTopic;
  String subTopic;
  String pubTopic;

  DynamicJsonDocument doc(256);
  _airConMqttSettingsService->read([&](AirConMqttSettings& settings) {
    configTopic = settings.mqttPath + "/config";
    subTopic = settings.mqttPath + "/set";
    pubTopic = settings.mqttPath + "/state";
    doc["~"] = settings.mqttPath;
    doc["name"] = settings.name;
    doc["unique_id"] = settings.uniqueId;
  });
  doc["cmd_t"] = "~/set";
  doc["stat_t"] = "~/state";
  doc["schema"] = "json";

  doc["onoff"] = false;
  doc["mode"] = "auto";
  doc["temperature"] = 24.0f;
  doc["flowspeed"] = 3;
  doc["verticalswing"] = false;
  doc["sensor_temp_inside"] = 24.0f;
  doc["sensor_temp_outside"] = 24.0f;

  String payload;
  serializeJson(doc, payload);
  _mqttClient->publish(configTopic.c_str(), 0, false, payload.c_str());

  _mqttPubSub.configureTopics(pubTopic, subTopic);
}
