#include <WString.h>

String GetWifiSSID();
String GetWifiPassword();
String GetHostname();
String GetMQTTHost();
String GetMQTTUser();
String GetMQTTPass();
String GetOTAPassword();

void SaveWifiSSID(String ssid);
void SaveWifiPassword(String pass);
void SaveHostname(String hostname);
void SaveMQTTHost(String hostname);
void SaveMQTTUser(String username);
void SaveMQTTPass(String password);
void SaveOTAPassword(String password);

void InitStorage();
