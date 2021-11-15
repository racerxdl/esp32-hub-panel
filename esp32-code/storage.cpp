#include "storage.h"
#include <EEPROM.h>

#define SSID_LENGTH 64
#define PASSWORD_LENGTH 32
#define HOSTNAME_LENGTH 16

struct Config {
  // WIFI
  char SSID[SSID_LENGTH];
  char WifiPassword[PASSWORD_LENGTH];
  char Hostname[HOSTNAME_LENGTH];
  char OTAPassword[PASSWORD_LENGTH];

  // MQTT
  char MQTTHostname[HOSTNAME_LENGTH];
  char MQTTUsername[PASSWORD_LENGTH];
  char MQTTPassword[PASSWORD_LENGTH];

} currentConfig;

const size_t ConfigLength = sizeof(currentConfig);

void ReadConfig() {
  char *c = (char *)(&currentConfig);
  for (int i=0; i<ConfigLength;i++) {
    c[i] = EEPROM.read(i);
  }

  // Pad all strings to be null terminated
  currentConfig.SSID[SSID_LENGTH-1] = 0x00;
  currentConfig.WifiPassword[PASSWORD_LENGTH-1] = 0x00;
  currentConfig.OTAPassword[PASSWORD_LENGTH-1] = 0x00;
  currentConfig.Hostname[HOSTNAME_LENGTH-1] = 0x00;
  currentConfig.MQTTHostname[HOSTNAME_LENGTH-1] = 0x00;
  currentConfig.MQTTUsername[PASSWORD_LENGTH-1] = 0x00;
  currentConfig.MQTTPassword[PASSWORD_LENGTH-1] = 0x00;
}

void SaveConfig() {
  currentConfig.SSID[SSID_LENGTH-1] = 0x00;
  currentConfig.WifiPassword[PASSWORD_LENGTH-1] = 0x00;
  currentConfig.OTAPassword[PASSWORD_LENGTH-1] = 0x00;
  currentConfig.Hostname[HOSTNAME_LENGTH-1] = 0x00;
  currentConfig.MQTTHostname[HOSTNAME_LENGTH-1] = 0x00;
  currentConfig.MQTTUsername[PASSWORD_LENGTH-1] = 0x00;
  currentConfig.MQTTPassword[PASSWORD_LENGTH-1] = 0x00;

  char *c = (char *)(&currentConfig);
  for (int i=0; i<ConfigLength;i++) {
    EEPROM.write(i, c[i]);
  }
  EEPROM.commit();
}

String GetWifiSSID() {
  return String(currentConfig.SSID);
}

String GetWifiPassword() {
  return String(currentConfig.WifiPassword);
}
String GetOTAPassword() {
  return String(currentConfig.OTAPassword);
}

String GetHostname() {
  return String(currentConfig.Hostname);
}

String GetMQTTHost() {
  return String(currentConfig.MQTTHostname);
}
String GetMQTTUser() {
  return String(currentConfig.MQTTUsername);
}
String GetMQTTPass() {
  return String(currentConfig.MQTTPassword);
}

void SaveWifiSSID(String ssid) {
    int maxLen = SSID_LENGTH-1;
    if (ssid.length() < maxLen) {
      maxLen = ssid.length();
    }
    for (int i = 0; i < SSID_LENGTH; i++) {
      if (i < maxLen) {
        currentConfig.SSID[i] = ssid[i];
      } else {
        currentConfig.SSID[i] = 0x00;
      }
    }
    SaveConfig();
}

void SaveWifiPassword(String pass) {
    int maxLen = PASSWORD_LENGTH-1;
    if (pass.length() < maxLen) {
      maxLen = pass.length();
    }
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
      if (i < maxLen) {
        currentConfig.WifiPassword[i] = pass[i];
      } else {
        currentConfig.WifiPassword[i] = 0x00;
      }
    }
    SaveConfig();
}


void SaveOTAPassword(String pass) {
    int maxLen = PASSWORD_LENGTH-1;
    if (pass.length() < maxLen) {
      maxLen = pass.length();
    }
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
      if (i < maxLen) {
        currentConfig.OTAPassword[i] = pass[i];
      } else {
        currentConfig.OTAPassword[i] = 0x00;
      }
    }
    SaveConfig();
}

void SaveHostname(String hostname) {
    int maxLen = HOSTNAME_LENGTH-1;
    if (hostname.length() < maxLen) {
      maxLen = hostname.length();
    }
    for (int i = 0; i < HOSTNAME_LENGTH; i++) {
      if (i < maxLen) {
        currentConfig.Hostname[i] = hostname[i];
      } else {
        currentConfig.Hostname[i] = 0x00;
      }
    }
    SaveConfig();
}

void InitStorage() {
  EEPROM.begin(ConfigLength);
  ReadConfig();
}

void SaveMQTTHost(String hostname) {
    int maxLen = HOSTNAME_LENGTH-1;
    if (hostname.length() < maxLen) {
      maxLen = hostname.length();
    }
    for (int i = 0; i < HOSTNAME_LENGTH; i++) {
      if (i < maxLen) {
        currentConfig.MQTTHostname[i] = hostname[i];
      } else {
        currentConfig.MQTTHostname[i] = 0x00;
      }
    }
    SaveConfig();
}
void SaveMQTTUser(String username) {
    int maxLen = PASSWORD_LENGTH-1;
    if (username.length() < maxLen) {
      maxLen = username.length();
    }
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
      if (i < maxLen) {
        currentConfig.MQTTUsername[i] = username[i];
      } else {
        currentConfig.MQTTUsername[i] = 0x00;
      }
    }
    SaveConfig();
}
void SaveMQTTPass(String password) {
    int maxLen = PASSWORD_LENGTH-1;
    if (password.length() < maxLen) {
      maxLen = password.length();
    }
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
      if (i < maxLen) {
        currentConfig.MQTTPassword[i] = password[i];
      } else {
        currentConfig.MQTTPassword[i] = 0x00;
      }
    }
    SaveConfig();
}
