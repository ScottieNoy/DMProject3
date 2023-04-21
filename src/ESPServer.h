#ifndef ESPSERVER_H
#define ESPSERVER_H

#include <ESPAsyncWebServer.h>
#include <Lock.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <SPIFFS.h>

#define BUTTONTIMEOUT 15000

class ESPServer {
public:
  ESPServer(int port, Lock& lock);
  void begin();
  
  
  
private:
  AsyncWebServer *_server;
  Lock& _lock;
  
  
  void generateHTML();
  
};

#endif // ESPSERVER_H
