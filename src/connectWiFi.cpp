// Libraries included:
#include "ConnectWiFi.h"

// Connects to the WiFi network with the provided SSID and password, and then prints the IP address.
// This function is defined in src/connectWiFi.cpp, declared in src/connectWiFi.h, and called in src/main.cpp
void ConnectWiFi::connect(char* ssid, char* password)
{
    // Start WiFi connection with provided SSID and password
    WiFi.begin(ssid, password);

    // Wait until the WiFi connection is established
    while (WiFi.status() != WL_CONNECTED)
    {
        // Delay for 500ms to avoid spamming the network
        delay(500);
        // Print a dot for each attempt to connect
        Serial.print(".");
    }
    
    // Print empty line for readability
    Serial.println("");
    
    // Print the SSID to which the device is connected
    Serial.print("Connected to ");
    Serial.println(ssid);
    
    // Print the IP address assigned to this device
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

// Checks if the device is connected to the WiFi network
bool ConnectWiFi::isConnected()
{
    // Returns true if the device is connected
    return WiFi.status() == WL_CONNECTED;
}

// Returns the IP address of the device as a string
String ConnectWiFi::getIP()
{
    // Converts the IP address from IPAddress format to a string format
    return IpAddress2String(WiFi.localIP());
}

// Converts an IPAddress object into a string
String ConnectWiFi::IpAddress2String(const IPAddress& ipAddress)
{
    // Returns the IP address as a string, with '.' as the separator between bytes
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}
