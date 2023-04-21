#include <Arduino.h>
#include "connectWiFi.h"
#include "ESPServer.h"

/// --------- WiFi credentials -------- ///

char* ssid = "Oscar";
char* password = "oscar12345";
ESPServer server(80, 10);

/// ---------------------------------- ///


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connect(ssid, password);
  server.begin();
  Serial.println("Server started");

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  Serial.println("Server running");
  delay(1000);
}