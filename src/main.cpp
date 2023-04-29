// ===================== Include Libraries ========================= //
#include <Arduino.h>
#include "ConnectWiFi.h"
#include "ESPServer.h"
#include "Lock.h"
#include "RFID.h"
#include "Controls.h"
#include "LCD.h"

// ===================== Motor Pin Definitions ===================== //

#define IN1 15                                                       // Motor Pin for IN1
#define IN2 2                                                        // Motor Pin for IN2
#define IN3 4                                                        // Motor Pin for IN3
#define IN4 16                                                       // Motor Pin for IN4

// ===================== LCD Pin Definitions ======================= //

#define LCD_ADDRESS 0x27                                             // LCD Address
#define LCD_COLUMNS 16                                               // LCD Columns
#define LCD_ROWS 2                                                   // LCD Rows

// SDA = 21, SCL = 22                                                // LCD Pin SDA and SCL

// ==================== Motor Speed Definitions ==================== //

#define stepsPerRevolultion 2048                                     // Steps per Revolution

// ===================== RFID Pin Definitions ====================== //

#define SS_PIN   5                                                   // RFID Pin
#define RST_PIN  16                                                  // RFID Pin

// ===================== Button Pin Definitions ==================== //

#define BUTTON 39                                                    // Button Pin
#define POT    36                                                    // Potentiometer Pin

// ===================== WiFi Definitions ========================== //

char* ssid = "homeweb";                                              // WiFi Name
char* password = "Samuel42";                                         // WiFi Password

// ===================== Global Variables ========================== //

Lock lock(stepsPerRevolultion, IN1, IN3, IN2, IN4);                  // Lock Object
ESPServer server(80, lock);                                          // Server Object
RFID rfid(lock,SS_PIN, RST_PIN);                                     // RFID Object
Controls control(POT, BUTTON, lock, rfid);                           // Controls Object
LCD lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);                         // LCD Object
ConnectWiFi wifi;                                                    // WiFi Object

// ===================== Setup and Loop ============================ //

void setup() {                                                       // Setup
  Serial.begin(115200);                                              // Start Serial Monitor
  lcd.setupLCD();                                                    // Setup LCD
  wifi.connect(ssid, password);                                  // Connect to WiFi
  while(wifi.isConnected() == false) {                               // While not connected to WiFi
    lcd.writeLCD("Connecting to", ssid);                             // Print to LCD
    delay(1000);                                                     // Wait 1000ms
  }
  lcd.writeLCD("Connected to", ssid);                                // Print to LCD
  delay(1000);                                                       // Wait 1000ms
  server.begin();                                                    // Start Server
  Serial.println("Server Running");                                  // Print to Serial Monitor
  lcd.writeLCD("Server Running", "");                                // Print to LCD

}

void loop() {                                                        // Loop
  rfid.RMain();                                                      // RFID Main
  lock.update();                                                     // Lock Update
  control.adjustMax();                                               // Adjust Max Speed
  if(!wifi.isConnected()) {                                          // If not connected to WiFi
    wifi.connect(ssid, password);                                // Connect to WiFi
  }
}
// ===================== End of File =============================== //
