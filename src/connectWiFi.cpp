#include "connectWiFi.h"
#include <WiFi.h>

// This connects to the WiFi network with the given credentials and prints the IP address.
// This function is defined in src/connectWiFi.cpp
// This function is declared in src/connectWiFi.h
// This function is called in src/main.cpp

void connect(char* ssid, char* password) { 
  WiFi.begin(ssid, password);                     // Connect to the network
  while (WiFi.status() != WL_CONNECTED) {         // Wait for the Wi-Fi to connect
    delay(500);                                   // Wait 500ms before retrying
    Serial.print(".");                            // Print a dot to show progress
  }
  Serial.println("");                             // Print a new line        
  Serial.print("Connected to ");                  // Print a message "Connected to"
  Serial.println(ssid);                           // Print the network name (SSID)
  Serial.print("IP address: ");                   // Print a message "IP address: "
  Serial.println(WiFi.localIP());                 // Print the IP address
}