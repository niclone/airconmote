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

  DynamicJsonDocument doc(2048);
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

  doc["dev_cla"] = "hvac";

  doc["temperature_unit"] = "C";
  doc["temp_step"] = 0.5;
  doc["precision"] = 0.5;
  doc["min_temp"] = 10;
  doc["max_temp"] = 30;

  JsonArray modes = doc.createNestedArray("modes");
  modes.add("off");
  modes.add("auto");
  modes.add("cool");
  modes.add("heat");
  modes.add("dry");
  modes.add("fan_only");


  JsonArray fanmodes = doc.createNestedArray("fan_modes");
  fanmodes.add("auto");
  fanmodes.add("quiet");
  fanmodes.add("low");
  fanmodes.add("gentle");
  fanmodes.add("medium");
  fanmodes.add("strong");
  fanmodes.add("high");

/*
  doc["power_command_topic"] = "~/set";
  doc["power_command_template"] = "{\"power\": {{ value }}}";
  //doc["power_state_topic"] = "~/state";
  //doc["power_state_template"] = "{{ value_json.power }}";
  doc["payload_on"] = "on";
  doc["payload_off"] = "off";
*/

  doc["temperature_command_topic"] = "~/set";
  doc["temperature_command_template"] = "{\"temperature\": {{ value }}}";
  doc["temperature_state_topic"] = "~/state";
  doc["temperature_state_template"] = "{{ value_json.temperature }}";

  doc["mode_command_topic"] = "~/set";
  doc["mode_command_template"] = "{\"mode\": \"{{ value }}\"}";
  doc["mode_state_topic"] = "~/state";
  doc["mode_state_template"] = "{{ value_json.mode }}";

  doc["current_temperature_topic"] = "~/state";
  doc["current_temperature_template"] = "{{ value_json.sensor_temp_inside }}";

  doc["fan_mode_command_topic"] = "~/set";
  doc["fan_mode_command_template"] = "{\"flowspeed\": \"{{ value }}\"}";
  doc["fan_mode_state_topic"] = "~/state";
  doc["fan_mode_state_template"] = "{{ value_json.flowspeed }}";

  doc["swing_mode_command_topic"] = "~/set";
  doc["swing_mode_command_template"] = "{\"verticalswing\": {{ 0 if value==\"off\" else 1 }}}";
  doc["swing_mode_state_topic"] = "~/state";
  doc["swing_mode_state_template"] = "{{ value_json.verticalswing }}";



  String payload;
  serializeJson(doc, payload);
  _mqttClient->publish(configTopic.c_str(), 0, false, payload.c_str());

  _mqttPubSub.configureTopics(pubTopic, subTopic);
}
