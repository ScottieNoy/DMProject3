// Liberies included:
#include "ConnectWiFi.h"

// This connects to the WiFi network with the given credentials and prints the IP address.
// This function is defined in src/connectWiFi.cpp
// This function is declared in src/connectWiFi.h
// This function is called in src/main.cpp

void ConnectWiFi::connect(char* ssid, char* password)
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

bool ConnectWiFi::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

IPAddress ConnectWiFi::getIP()
{
    return WiFi.localIP();
}