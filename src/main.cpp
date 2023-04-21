#include <Arduino.h>
#include "connectWiFi.h"
#include "ESPServer.h"
#include "Lock.h"

#define IN1 17
#define IN2 5
#define IN3 18
#define IN4 19
#define stepsPerRevolultion 2048

/// --------- WiFi credentials -------- ///

char* ssid = "Oscar";
char* password = "oscar12345";
Lock lock(stepsPerRevolultion, IN1, IN3, IN2, IN4);
// ESPServer server(80, lock);


/// ---------------------------------- ///


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connect(ssid, password);
  // server.begin();
  // Serial.println("Server started");
  lock.unlock();

}

void loop() {
  // put your main code here, to run repeatedly:
}