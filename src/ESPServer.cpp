#include "ESPServer.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <SPIFFS.h>

ESPServer::ESPServer(int port) {
  _server = new WebServer(port);
}

void ESPServer::begin() {
  _server->on("/", std::bind(&ESPServer::generateHTML, this));
  _server->begin();
}

void ESPServer::handleClient() {
  _server->handleClient();
}

void ESPServer::generateHTML() {
  // Open the index.html file
  if(!SPIFFS.begin(true)){
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }
  File htmlFile = SPIFFS.open("/WebServerDesign.html", "r");
  if (!htmlFile) {
    Serial.println("Failed to open WebServerDesign.html");
    return;
  }

  // Read the contents of the file into a string
  String htmlString = "";
  while (htmlFile.available()) {
    htmlString += htmlFile.readString();
  }
  // Get current date and time from NTP server
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org");
  timeClient.begin();
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  
  htmlString += "<hr>";
  
  htmlString += "<p>Last updated: " + String(ptm->tm_mday) + "/" + String(ptm->tm_mon) + "/" + String(ptm->tm_year + 1900) + " at " + String(ptm->tm_hour+2) + ":" + String(ptm->tm_min)+ ":" + String(ptm->tm_sec) + "</p>";
  
  htmlString += "</body></html>";

  // Send the HTML string as a response to the HTTP request
  _server->send(200, "text/html", htmlString);
}


