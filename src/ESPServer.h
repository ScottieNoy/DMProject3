// This is to make sure we don't declare the function more than once by including the header multiple times:
#ifndef ESPSERVER_H   
#define ESPSERVER_H

// Liberies included:
#include <ESPAsyncWebServer.h>
#include <Lock.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <SPIFFS.h>
#include <RFID.h>

#define BUTTONTIMEOUT 15000             // defining the timeout for the button to be pressed in milliseconds (ms) (15 seconds)

class ESPServer {                       // This is the ESPServer class.
public:                                 // It has two public functions:
  ESPServer(int port, Lock& lock, RFID& rfid);      // The constructor
  void begin();                         // The begin function
  
  
  
private:                                // It has three private fields:
  AsyncWebServer *_server;              // A pointer to an AsyncWebServer object
  Lock& _lock;                          // A reference to a Lock object
  RFID& _rfid;                          // A reference to a RFID object
  
  
  void addNewTag();
  
};

#endif // ESPSERVER_H                   // This is to make sure we don't declare the function more than once by including the header multiple times.
