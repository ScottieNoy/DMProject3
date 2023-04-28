// Liberies included:
#include "ESPServer.h"


// This is the constructor for the ESPServer class.
// It takes a port number and a reference to a Lock object.
// It creates a new AsyncWebServer object and stores it in the _server field.
// It also stores the reference to the Lock object in the _lock field.

// This constructor is defined in src/ESPServer.cpp
// This constructor is declared in src/ESPServer.h
// This constructor is called in src/main.cpp

ESPServer::ESPServer(int port, Lock& lock)            // Constructor
  : _server(new AsyncWebServer(port)), _lock(lock) { 
}

void ESPServer::begin() {                             // This function creates the routes for the web server.
  
    String html = "<!DOCTYPE html>";                  // This is the HTML for the web page.
    html += "<html>";                                 // It is stored in a String object.
    html += "<head>";                                 // And it is sent to the client when they visit the root path.
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
    html += "<button id=\"unlock-btn\" onclick=\"unlock()\">Unlock</button>";     // This is the button that unlocks the lock.
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
    html += "  }, " + String(BUTTONTIMEOUT) + ");";           // This is the timeout value. It is stored in the BUTTONTIMEOUT constant.
    html += "}";
    html += "</script>";
    html += "</body>";
    html += "</html>";





    _server->on("/", HTTP_GET, [html](AsyncWebServerRequest *request) {     // This is the route for the root path.
        request->send(200, "text/html", html);                              // It sends the HTML to the client.
        // Serial.println("Someone visited the main");                      // And it prints a message to the serial monitor. (used for debugging)
    });

    _server->on("/unlock", HTTP_GET, [&](AsyncWebServerRequest *request) {  // This is the route for the /unlock path.
        Serial.println("Unlocked using webserver");                     // And it prints a message to the serial monitor. (used for debugging)
        _lock.unlock();                                                     // It calls the unlock() method on the Lock object.
        request->send(200, "text/plain", "OK");                             // And it sends a 200 (not a 404 error page) OK response to the client.

    });
  _server->begin();                                                         // This starts the web server.
}
