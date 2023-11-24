#include <DebugConsole.h>

WiFiClient D;

void debugloop() {
#ifdef DEBUGCONSOLE
    if (WiFi.status() == WL_CONNECTED) {
        if (!D.connected()) {
            //Serial.println("Connecting to debug console...");
            D.connect(DEBUGCONSOLE, 5555);
            if (D.connected()) {
                //Serial.println("Connected to debug console.");
                D.println("Welcome to AirconMote debug console!");
            }
        }
    }
#endif
}

