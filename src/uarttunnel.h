// config: ////////////////////////////////////////////////////////////
//

//#include <esp_wifi.h>
//#include <WiFi.h>
#if defined(ESP8266)
#include <SoftwareSerial.h>
#endif

#define PROTOCOL_TCP

bool uarttunnel_debug = true;

/*************************  COM Port 2 *******************************/
#define UART_BAUD2 2400            // Baudrate UART2
//#define SERIAL_PARAM2 SERIAL_8N2    // Data/Parity/Stop UART2
#define SERIAL2_RXPIN D7             // receive Pin UART2
#define SERIAL2_TXPIN D8             // transmit Pin UART2
#define SERIAL2_TCP_PORT 8882       // Wifi Port UART2

#define bufferSize 1024

//////////////////////////////////////////////////////////////////////////

