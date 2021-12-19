#include <Arduino.h>
#if defined(ESP8266)
#include <SoftwareSerial.h>
#endif

#include "uarttunnel.h"
//#include <esp_wifi.h>
//#include <WiFi.h>

//HardwareSerial Serial_two(2);
#if defined(ESP8266)
SoftwareSerial Serial_two(SERIAL2_RXPIN, SERIAL2_TXPIN, false);
#elif defined(ESP32)
HardwareSerial Serial_two(1);
#endif

//SoftwareSerial Serial_two(SERIAL2_TXPIN, SERIAL2_RXPIN, false);

#define MAX_NMEA_CLIENTS 4
#ifdef PROTOCOL_TCP
#include <WiFiClient.h>
#include <WiFiServer.h>
WiFiServer server_2(SERIAL2_TCP_PORT);
WiFiClient TCPClient[MAX_NMEA_CLIENTS];
#endif


uint8_t buf1[bufferSize];
uint16_t i1=0;

uint8_t buf2[bufferSize];
uint16_t i2=0;

uint8_t BTbuf[bufferSize];
uint16_t iBT =0;


void uarttunnel_setup() {

  delay(500);

#if defined(ESP8266)
  Serial_two.begin(UART_BAUD2, SWSERIAL_8N2, SERIAL2_RXPIN, SERIAL2_TXPIN, false, 256, 1024);
#elif defined(ESP32)
  Serial_two.begin(UART_BAUD2, SERIAL_8N2, 16, 17, false, 200000UL);
#endif

  if(uarttunnel_debug) Serial.println("\n\nLK8000 WiFi serial bridge V1.00");

  #ifdef PROTOCOL_TCP
  Serial.println("Starting TCP Server 2");
  server_2.begin(); // start TCP server
  server_2.setNoDelay(true);
  #endif

  esp_err_t esp_wifi_set_max_tx_power(50);  //lower WiFi Power

}


void uarttunnel_loop() {
    if (server_2.hasClient()) {
      for(byte i = 0; i < MAX_NMEA_CLIENTS; i++){
        //find free/disconnected spot
        if (!TCPClient[i] || !TCPClient[i].connected()) {
          if(TCPClient[i]) TCPClient[i].stop();
          TCPClient[i] = server_2.available();
          TCPClient[i].write("Hello new client ! welcome ! yeah ! that's great !\n");
          if(uarttunnel_debug) Serial.println("New client.");
          continue;
        }
      }
      //no free/disconnected spot so reject
      WiFiClient TmpserverClient = server_2.available();
      TmpserverClient.stop();
    }

      for(byte cln = 0; cln < MAX_NMEA_CLIENTS; cln++)
      {
        if(TCPClient[cln])
        {
          while(TCPClient[cln].available())
          {
            buf1[i1] = TCPClient[cln].read(); // read char from client (LK8000 app)
            if(i1<bufferSize-1) i1++;
          }

          Serial_two.write(buf1, i1); // now send to UART(num):
          i1 = 0;
        }
      }

      if(Serial_two.available())
      {
        while(Serial_two.available())
        {
          buf2[i2] = Serial_two.read(); // read char from UART(num)
          if(i2<bufferSize-1) i2++;
        }
        // now send to WiFi:
        for(byte cln = 0; cln < MAX_NMEA_CLIENTS; cln++)
        {
          if(TCPClient[cln])
            TCPClient[cln].write(buf2, i2);
        }
        i2 = 0;
      }

}

