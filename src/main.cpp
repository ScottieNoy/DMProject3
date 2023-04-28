#include <Arduino.h>
#include "connectWiFi.h"
#include "ESPServer.h"
#include "Lock.h"
#include "RFID.h"
#include "Controls.h"

#define IN1 4
#define IN2 16
#define IN3 17
#define IN4 5
// #define IN1 2
// #define IN2 0
// #define IN3 4
// #define IN4 5
#define stepsPerRevolultion 2048

//RFID pins
#define SS_PIN 21
#define RST_PIN 22
#define POT 36

// Button pin
#define BUTTON 39

/// --------- WiFi credentials -------- ///

char* ssid = "Oscar";
char* password = "oscar12345";
Lock lock(stepsPerRevolultion, IN1, IN3, IN2, IN4);
ESPServer server(80, lock);
RFID rfid(lock,SS_PIN, RST_PIN);
Controls control(POT, BUTTON, lock, rfid);

/// ---------------------------------- ///


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connect(ssid, password);
  server.begin();
  Serial.println("Server Running");
}

void loop() {
  rfid.RMain();
  lock.update();
  control.adjustMax();
  if(!isConnected()) {
    connect(ssid, password);
  }
}