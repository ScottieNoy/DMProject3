#include "ESPServer.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

ESPServer::ESPServer(int port, int maxVisitors) {
  _currentVisitors = 0;
  _maxVisitors = maxVisitors;
  _server = new WebServer(port);
}

void ESPServer::begin() {
  _server->on("/", std::bind(&ESPServer::generateHTML, this));
  _server->begin();
}

void ESPServer::handleClient() {
  _server->handleClient();
}

void ESPServer::updateNum(int amount) {
  _currentVisitors = amount;
  generateHTML();
}

void ESPServer::updateMax(int amount) {
  _maxVisitors = amount;
  generateHTML();
}

void ESPServer::addVisitor(String name) {
  if (_numVisitors < _maxVisitors) {
    _visitors[_numVisitors++] = name;
    generateHTML();
  }
}

void ESPServer::removeVisitor(String name) {
  for (int i = 0; i < _numVisitors; i++) {
    if (_visitors[i] == name) {
      for (int j = i; j < _numVisitors - 1; j++) {
        _visitors[j] = _visitors[j+1];
      }
      _numVisitors--;
      generateHTML();
      break;
    }
  }
}

void ESPServer::generateHTML() {
  String html = "<html><head><title>Visitors</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>body {font-family: Arial, Helvetica, sans-serif;}</style>";
  html += "</head><body>";
  
  html += "<h1>Current Visitors</h1>";
  html += "<div style='display:flex;justify-content:space-between;'>";
  html += "<div style='text-align:center;'><h2>" + String(_currentVisitors) + "</h2><p>Current</p></div>";
  html += "<div style='text-align:center;'><h2>" + String(_maxVisitors) + "</h2><p>Maximum</p></div>";
  html += "</div>";
  
  html += "<hr>";
  
  html += "<h2>Visitors:</h2>";
  if (_numVisitors > 0) {
    html += "<ul style='list-style:none;padding:0;'>";
    for (int i = 0; i < _numVisitors; i++) {
      html += "<li style='border-bottom:1px solid #ccc;padding:5px;'>" + _visitors[i] + "</li>";
    }
    html += "</ul>";
  } else {
    html += "<p>No visitors at the moment.</p>";
  }
  
  // Get current date and time from NTP server
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org");
  timeClient.begin();
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  
  html += "<hr>";
  
  html += "<p>Last updated: " + String(ptm->tm_mday) + "/" + String(ptm->tm_mon) + "/" + String(ptm->tm_year + 1900) + " at " + String(ptm->tm_hour+2) + ":" + String(ptm->tm_min)+ ":" + String(ptm->tm_sec) + "</p>";
  
  html += "</body></html>";
  
  _server->send(200, "text/html", html);
}


