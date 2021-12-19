#ifndef AirConStateService_h
#define AirConStateService_h

#include <AirConMqttSettingsService.h>

#include <HttpEndpoint.h>
#include <MqttPubSub.h>
#include <WebSocketTxRx.h>
#include "AirConState.h"
#include "AirConDevice.h"

#define AIRCON_SETTINGS_ENDPOINT_PATH "/rest/airConState"
#define AIRCON_SETTINGS_SOCKET_PATH "/ws/airConState"


class AirConStateService : public StatefulService<AirConState> {
 public:
  AirConStateService(AsyncWebServer* server,
                    SecurityManager* securityManager,
                    AsyncMqttClient* mqttClient,
                    AirConMqttSettingsService* airConMqttSettingsService);
  void begin();
  void loop();

 private:
  AirConDevice *aircondevice;
  HttpEndpoint<AirConState> _httpEndpoint;
  MqttPubSub<AirConState> _mqttPubSub;
  WebSocketTxRx<AirConState> _webSocket;
  AsyncMqttClient* _mqttClient;
  AirConMqttSettingsService* _airConMqttSettingsService;

  void registerConfig();
  void onConfigUpdated();
};

#endif
