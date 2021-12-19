#include <AirConMqttSettingsService.h>

AirConMqttSettingsService::AirConMqttSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(AirConMqttSettings::read,
                  AirConMqttSettings::update,
                  this,
                  server,
                  AIRCON_BROKER_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(AirConMqttSettings::read, AirConMqttSettings::update, this, fs, AIRCON_BROKER_SETTINGS_FILE) {
}

void AirConMqttSettingsService::begin() {
  _fsPersistence.readFromFS();
}
