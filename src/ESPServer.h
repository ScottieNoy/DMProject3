#ifndef ESPSERVER_H
#define ESPSERVER_H

#include <ESPAsyncWebServer.h>
#include <Lock.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <SPIFFS.h>

class ESPServer {
public:
  ESPServer(int port, Lock& lock);
  void begin();
  void handleClient();
  
  
  
private:
  AsyncWebServer *_server;
  Lock& _lock;
  
  
  void generateHTML();
  
};

#endif // ESPSERVER_H
