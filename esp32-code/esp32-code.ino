#include "storage.h"
#include "ledcontroller.h"
#include "wifi.h"

// ESP32 to Panel pinout at ledcontroller.h

void setup() {
  Serial.begin(115200);
  InitStorage();

  utc = -3; // GMT-3 Brazil

  SaveWifiSSID("YourSSID");
  SaveWifiPassword("YourWifiPassword");
  SaveHostname("ESP32-Panel");
  SaveOTAPassword("YourOTAPassword");
  SetupLeds();
  SetupWiFi();
  delay(1000);
  Clear();
  SetMode(MODE_CLOCK);
}

void loop() {
  WiFiLoop();
  LedLoop();
}
