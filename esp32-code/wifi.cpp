#include <NTPClient.h>
#include <WiFiUdp.h>
#include <AsyncUDP.h>
#include "ledcontroller.h"
#include "wifi.h"
#include "storage.h"

String ssid     = "";
String password = "";
String otaPassword = "";
String hostname = "";

WiFiUDP ntpUDP;

int16_t utc = -3; //UTC -3:00 Brazil
uint32_t currentMillis = 0;
uint32_t previousMillis = 0;

NTPClient timeClient(ntpUDP, "pool.ntp.br", utc*3600, 60000);

AsyncUDP udp;

struct PanelPacket {
  uint16_t lineN;
  uint16_t pixels[64];
};

long lastUpdate = 0;

bool inOTA = false;

int getHours() {
  return timeClient.getHours();
}

int getMinutes() {
  return timeClient.getMinutes();
}

int getSeconds() {
  return timeClient.getSeconds();
}

void SetupWiFi() {
  ssid = GetWifiSSID();
  password = GetWifiPassword();
  hostname = GetHostname();
  inOTA = false;
  Serial.println("\r\n");
  Serial.print("Chip ID: 0x");
  Serial.println((long int)ESP.getEfuseMac(), HEX);

  // Set Hostname.
  if (hostname == "") {
    hostname = "WIMATRIX-";
    hostname += String((long int)ESP.getEfuseMac(), HEX);
  }
  Serial.print("Hostname: ");
  Serial.println(hostname);

  Serial.println("Setting up WiFi");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);

  String tmp = "WiFi (";
  tmp += ssid;
  tmp += ")";

  LedPrint(tmp.c_str(), CRGB::Red);

  if (WiFi.getMode() != WIFI_STA) {
    WiFi.mode(WIFI_STA);
    LedLoop();
    delay(10);
  }

  if (WiFi.SSID() != ssid || WiFi.psk() != password) {
    Serial.println("WiFi config changed.");
    // ... Try to connect to WiFi station.
    WiFi.begin(ssid.c_str(), password.c_str());

    // ... Pritn new SSID
    Serial.print("new SSID: ");
    Serial.println(WiFi.SSID());
  } else {
    WiFi.begin();
  }

  WiFi.setHostname(hostname.c_str());
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(100);
    Serial.write('.');
    LedLoop();
    yield();
  }

  Serial.println();

  // Check connection
  if (WiFi.status() == WL_CONNECTED) {
    // ... print IP Address
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    tmp = "WiFi (";
    tmp += ssid;
    tmp += ")";

    LedPrint(tmp.c_str(), CRGB::Green);
    LedPrint("\n", CRGB::Green);
    LedPrint(WiFi.localIP().toString().c_str(), CRGB::Yellow);
    delay(1500);
  } else {
    LedPrint("WiFi", CRGB::Yellow);
    Serial.println("Can not connect to WiFi station. Go into AP mode.");

    // Go into software AP mode.
    WiFi.mode(WIFI_AP);

    delay(10);

    WiFi.softAP("LABMATRIX", "1234567890");

    delay(1500);

    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    LedPrint(WiFi.softAPIP().toString().c_str(), CRGB::Yellow);
  }

  // Start OTA server.
  ArduinoOTA.setHostname((const char *)hostname.c_str());

  ArduinoOTA.onStart([]() {
    SetMode(BACKGROUND_STRING_DISPLAY);
    SetBrightness(0.5);
    inOTA = true;
    Serial.println("OTA Update Start");
    //LedPrint("OTA", CRGB::Yellow);
    StartOTA();
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("OTA Update End");
    LedPrint("OTA", CRGB::Green);
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    String progressStr;
    unsigned int progressPercent = (progress / (total / 100));
    progressStr = String(progressPercent) + String("%");
    UpdateOTA(progressPercent);
    Serial.print("Progress: ");
    Serial.println(progressStr);
    //LedPrint(progressStr.c_str(), CRGB::Yellow);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    LedPrint("OTA", CRGB::Red);
    Serial.printf("Erro [%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Start Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connection failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Error");
    else if (error == OTA_END_ERROR) Serial.println("End Fail");
  });

  ArduinoOTA.begin();

  if (udp.listen(1234)) {
    udp.onPacket([](AsyncUDPPacket packet) {
      int numPanelPackets = packet.length() / sizeof(PanelPacket);
      PanelPacket *pkt = (PanelPacket *)(packet.data());
      
      for (int p = 0; p < numPanelPackets; p++) {
        SetBitmapLine(pkt[p].lineN, pkt[p].pixels);
      }
    });
  }

  // NTP
  timeClient.begin();
  timeClient.update();
}

void WiFiLoop() {
  if (millis() - lastUpdate > 30000) {
    timeClient.update();
    lastUpdate = millis();
  }
  ArduinoOTA.handle();
}

bool InOTA() {
  return inOTA;
}
