// This is to make sure we don't declare the function more than once by including the header multiple times:
#ifndef CONNECTWIFI_H
#define CONNECTWIFI_H

#include <WiFi.h>
#include <Arduino.h>
#include <IPAddress.h>

class ConnectWiFi {

public:
  void connect(char* ssid, char* password);
  bool isConnected();
  IPAddress getIP();

private:
  char* ssid;
  char* password;

};
#endif // CONNECTWIFI_H
