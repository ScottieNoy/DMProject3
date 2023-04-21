#include "ESPServer.h"



ESPServer::ESPServer(int port, Lock& lock)
  : _server(new AsyncWebServer(port)), _lock(lock) {
}

void ESPServer::begin() {
  _server->begin();
  if(!SPIFFS.begin(true)){
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }

  // Route for root / web page
  _server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/WebServerDesign.html", String(), false);
  });
  
  // Route to load style.css file
  _server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
}


