#include "Arduino.h"
#include <ArduinoOTA.h>

#ifndef __WIFI__
#define __WIFI__
void SetupWiFi();
int getHours();
int getMinutes();
int getSeconds();
bool InOTA();

void WiFiLoop();
#endif
