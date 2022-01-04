#include <AirConStateService.h>

#include "AirConDaikin.h"

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
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttPubSub(AirConState::haRead, AirConState::haUpdate, this, mqttClient),
    _webSocket(AirConState::read,
               AirConState::update,
               this,
               server,
               AIRCON_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED),
    _mqttClient(mqttClient),
    _airConMqttSettingsService(airConMqttSettingsService) {
  // configure led to be output
  //pinMode(LED_PIN, OUTPUT);

  // configure MQTT callback
  _mqttClient->onConnect(std::bind(&AirConStateService::registerConfig, this));

  // configure update handler for when the airCon settings change
  _airConMqttSettingsService->addUpdateHandler([&](const String& originId) { registerConfig(); }, false);

  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void AirConStateService::begin() {
  aircondevice = (AirConDevice *)new AirConDaikin();
  _state.onoff = DEFAULT_ONOFF;
  _state.mode = DEFAULT_MODE;
  _state.temperature = DEFAULT_TEMPERATURE;
  _state.flowspeed = DEFAULT_FLOWSPEED;
  _state.verticalswing = DEFAULT_VERTICALSWING;
  //onConfigUpdated();
}

void AirConStateService::loop() {
    aircondevice->loop();
}

void AirConStateService::onConfigUpdated() {
  //digitalWrite(LED_PIN, _state.ledOn ? LED_ON : LED_OFF);
  aircondevice->setState(&_state);
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
  doc["brightness"] = false;

  String payload;
  serializeJson(doc, payload);
  _mqttClient->publish(configTopic.c_str(), 0, false, payload.c_str());

  _mqttPubSub.configureTopics(pubTopic, subTopic);
}
