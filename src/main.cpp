#include <Arduino.h>
#include "connectWiFi.h"
#include "generateWebServer.h"
#include <WebServer.h>

/// --------- WiFi credentials -------- ///

char* ssid = "Sophus - iPhone";
char* password = "sophus123";

/// ---------------------------------- ///

/// --------- WebServer Port -------- ///

WebServer server(80);

/// --------------------------------- ///


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connect(ssid, password);
  generateWebServer();

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}