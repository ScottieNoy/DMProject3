#include "ESPServer.h"



ESPServer::ESPServer(int port, Lock& lock)
  : _server(new AsyncWebServer(port)), _lock(lock) {
}

void ESPServer::begin() {
  
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<title>My Lock Web Server</title>";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    html += "<style>";
    html += "body {";
    html += "  font-family: Arial, Helvetica, sans-serif;";
    html += "  background-color: #f2f2f2;";
    html += "}";
    html += ".container {";
    html += "  margin: auto;";
    html += "  max-width: 600px;";
    html += "  padding: 20px;";
    html += "  background-color: white;";
    html += "  box-shadow: 0px 0px 10px 0px rgba(0,0,0,0.2);";
    html += "}";
    html += ".header {";
    html += "  text-align: center;";
    html += "}";
    html += "h1 {";
    html += "  font-size: 2.5rem;";
    html += "}";
    html += ".button-container {";
    html += "  display: flex;";
    html += "  justify-content: center;";
    html += "}";
    html += "button {";
    html += "  background-color: #4CAF50;";
    html += "  color: white;";
    html += "  padding: 15px 32px;";
    html += "  text-align: center;";
    html += "  text-decoration: none;";
    html += "  display: inline-block;";
    html += "  font-size: 1.5rem;";
    html += "  margin: 10px;";
    html += "  border-radius: 5px;";
    html += "  border: none;";
    html += "  cursor: pointer;";
    html += "}";
    html += "button:disabled {";
    html += "  background-color: gray;";
    html += "  cursor: not-allowed;";
    html += "}";
    html += ".description {";
    html += "  text-align: center;";
    html += "}";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<div class=\"container\">";
    html += "<div class=\"header\">";
    html += "<h1>My Lock Web Server</h1>";
    html += "</div>";
    html += "<div class=\"description\">";
    html += "<p>This web server allows you to unlock and lock a stepper motor-based lock.</p>";
    html += "</div>";
    html += "<div class=\"button-container\">";
    html += "<button id=\"unlock-btn\" onclick=\"unlock()\">Unlock</button>";
    html += "</div>";
    html += "</div>";
    html += "<script>";
    html += "function unlock() {";
    html += "  var btn = document.getElementById('unlock-btn');";
    html += "  if (btn.disabled) { return; }";
    html += "  btn.innerHTML = 'Unlocking...';";
    html += "  btn.disabled = true;";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('GET', '/unlock', true);";
    html += "  xhr.send();";
    html += "  setTimeout(function() {";
    html += "    btn.innerHTML = 'Unlock';";
    html += "    btn.disabled = false;";
    html += "  }, " + String(BUTTONTIMEOUT) + ");";
    html += "}";
    html += "</script>";
    html += "</body>";
    html += "</html>";





    _server->on("/", HTTP_GET, [html](AsyncWebServerRequest *request) {
        request->send(200, "text/html", html);
        // Serial.println("Someone visited the main");
    });

    _server->on("/unlock", HTTP_GET, [&](AsyncWebServerRequest *request) {
        _lock.unlock();
        request->send(200, "text/plain", "OK");
        // Serial.println("Someone unlocked the door");

    });
  _server->begin();
}
