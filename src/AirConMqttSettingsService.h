#ifndef AirConMqttSettingsService_h
#define AirConMqttSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <SettingValue.h>

#define AIRCON_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define AIRCON_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class AirConMqttSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;

  static void read(AirConMqttSettings& settings, JsonObject& root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }

  static StateUpdateResult update(JsonObject& root, AirConMqttSettings& settings) {
    settings.mqttPath = root["mqtt_path"] | SettingValue::format("homeassistant/airCon/#{unique_id}");
    settings.name = root["name"] | SettingValue::format("airCon-#{unique_id}");
    settings.uniqueId = root["unique_id"] | SettingValue::format("airCon-#{unique_id}");
    return StateUpdateResult::CHANGED;
  }
};

class AirConMqttSettingsService : public StatefulService<AirConMqttSettings> {
 public:
  AirConMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<AirConMqttSettings> _httpEndpoint;
  FSPersistence<AirConMqttSettings> _fsPersistence;
};

#endif  // end AirConMqttSettingsService_h
