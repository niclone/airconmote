#include <ESP8266React.h>
#include <LightMqttSettingsService.h>
#include <LightStateService.h>
#include <AirConMqttSettingsService.h>
#include <AirConStateService.h>

#include <AirConDaikin.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);

LightMqttSettingsService lightMqttSettingsService =
    LightMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
LightStateService lightStateService = LightStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &lightMqttSettingsService);




AirConMqttSettingsService airConMqttSettingsService =
    AirConMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());

AirConStateService airConStateService = AirConStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &airConMqttSettingsService);

/*
void uarttunnel_setup();
void uarttunnel_loop();
*/

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework and demo project
  esp8266React.begin();

  // load the initial light settings
  lightStateService.begin();

  // start the light service
  lightMqttSettingsService.begin();

  // load the initial airCon settings
  airConStateService.begin();

  // start the airCon service
  airConMqttSettingsService.begin();

  // start the server
  server.begin();

  //uarttunnel_setup();
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();

  airConStateService.loop();

  //uarttunnel_loop();
}
