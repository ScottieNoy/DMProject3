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

#define SS_PIN   5                                                   // RFID Pin (SDA)
#define SCK_PIN  18                                                  // RFID Pin (SCK)
#define RST_PIN  17                                                  // RFID Pin

// MOSI = 23, MISO = 19                                              // RFID Pin MOSI, MISO

// ===================== Button Pin Definitions ==================== //

#define BUTTON 32                                                    // Button Pin
#define POT    36                                                    // Potentiometer Pin

// ===================== WiFi Definitions ========================== //

char* ssid = "Oscar";                                                // WiFi Name
char* password = "oscar12345";                                       // WiFi Password

// ====================== Global Objects =========================== //

Lock lock(stepsPerRevolultion, IN1, IN3, IN2, IN4);                  // Lock Object
RFID rfid(lock,SS_PIN, RST_PIN);                                     // RFID Object
ESPServer server(80, lock, rfid);                                    // Server Object                                    // RFID Object
Controls control(POT, BUTTON, lock, rfid);                           // Controls Object
LCD lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);                         // LCD Object
ConnectWiFi wifi;                                                    // WiFi Object

// ===================== Setup and Loop ============================ //

void setup() {                                                       // Setup
  Serial.begin(115200);                                              // Start Serial Monitor
  lcd.setupLCD();                                                    // Setup LCD
  lcd.writeLCD("Starting", "System");                                // Print to LCD
  wifi.connect(ssid, password);                                      // Connect to WiFi
  while(wifi.isConnected() == false) {                               // While not connected to WiFi
    lcd.writeLCD("Connecting to", ssid);                             // Print to LCD
    delay(1000);                                                     // Wait 1000ms
  }
  lcd.writeLCD("Connected to", ssid);                                // Print to LCD
  delay(1000);                                                       // Wait 1000ms
  server.begin();                                                    // Start Server
  Serial.println("Server Running: " + wifi.getIP());                 // Print to Serial Monitor
  lcd.writeLCD("Server Running:", wifi.getIP());                     // Print to LCD
  delay(1000);                                                       // Wait 1000ms

}

void loop() {                                                        // Loop
  rfid.RMain();                                                      // RFID Main
  lock.update();                                                     // Lock Update
  control.adjustMax();                                               // Adjust Max Speed
  if(!wifi.isConnected()) {                                          // If not connected to WiFi
    wifi.connect(ssid, password);                                    // Connect to WiFi
  }
}
// ===================== End of File =============================== //
