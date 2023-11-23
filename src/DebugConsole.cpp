#include <DebugConsole.h>

WiFiClient D;

void debugloop() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!D.connected()) {
            //Serial.println("Connecting to debug console...");
            D.connect("192.168.168.10", 5555);
            //debugclient.connect(IPAddress('192.168.168.10'), 5555);
            if (D.connected()) {
                //Serial.println("Connected to debug console.");
                D.println("Welcome to AirconMote debug console!");
            }
        }
    }
}

