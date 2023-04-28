// This is to make sure we don't declare the function more than once by including the header multiple times:
#ifndef CONNECTWIFI_H 
#define CONNECTWIFI_H

// Liberies included:
#include "connectWiFi.h"
#include <WiFi.h>

class connectWiFi {

public:
  connectWiFi(char* ssid, char* password);
  void connect(char* ssid, char* password);
  bool isConnected();
  char getIP();

private:
    char* ssid;
    char* password;
};
#endif // CONNECTWIFI_H
