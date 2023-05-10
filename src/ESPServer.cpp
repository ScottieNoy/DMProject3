// Liberies included:
#include "ESPServer.h"


// This is the constructor for the ESPServer class.
// It takes a port number and a reference to a Lock object.
// It creates a new AsyncWebServer object and stores it in the _server field.
// It also stores the reference to the Lock object in the _lock field.

// This constructor is defined in src/ESPServer.cpp
// This constructor is declared in src/ESPServer.h
// This constructor is called in src/main.cpp

ESPServer::ESPServer(int port, Lock& lock, RFID& rfid)            // Constructor
  : _server(new AsyncWebServer(port)), _lock(lock), _rfid(rfid) { 
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
    html += "table {";
    html += "  width: 100%;";
    html += "  border-collapse: collapse;";
    html += "}";
    html += "th, td {";
    html += "  border: 1px solid black;";
    html += "  padding: 8px;";
    html += "  text-align: left;";
    html += "}";
    html += "th {";
    html += "  background-color: #f2f2f2;";
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
    html += "<p>Current visitors: <span id=\"visitors\">0</span> / <span id=\"max-visitors\">0</span></p>";
    html += "</div>";
    html += "<div class=\"button-container\">";
    html += "<button id=\"unlock-btn\" onclick=\"unlock()\">Unlock</button>";     // This is the button that unlocks the lock.
    html += "</div>";
    html += "<div class=\"button-container\">";
    html += "<button id=\"add-tag-btn\" onclick=\"addTag()\">Add Tag</button>"; // This is the new button for adding a new RFID tag.
    html += "</div>";
    html += "</div>";
    html += "<h2>Authorized Tags</h2>";
    html += "<table id=\"authorized-tags\">";
    html += "<thead>";
    html += "<tr><th>Tag ID</th></tr>";
    html += "</thead>";
    html += "<tbody></tbody>";
    html += "</table>";
    html += "<h2>Active Tags</h2>";
    html += "<table id=\"active-tags\">";
    html += "<thead>";
    html += "<tr><th>Tag ID</th></tr>";
    html += "</thead>";
    html += "<tbody></tbody>";
    html += "</table>";
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
    // html += "function addTag() {";
    // html += "  var xhr = new XMLHttpRequest();";
    // html += "  xhr.open('GET', '/add-tag', true);"; // This is the path that the server will listen to for adding a new tag.
    // html += "  xhr.send();";
    // html += "}";
    html += "function updateTables() {";
    html += "  fetch('/authorized-tags')";
    html += "    .then(response => response.json())";
    html += "    .then(tags => updateTable('authorized-tags', tags));";
    html += "  fetch('/active-tags')";
    html += "    .then(response => response.json())";
    html += "    .then(tags => updateTable('active-tags', tags));";
    html += "}";
    html += "function updateTable(tableId, tags) {";
    html += "  const table = document.getElementById(tableId);";
    html += "  const tbody = table.querySelector('tbody');";
    html += "  tbody.innerHTML = '';";
    html += "  for (const tag of tags) {";
    html += "    const tr = document.createElement('tr');";
    html += "    const td = document.createElement('td');";
    html += "    td.textContent = tag;";
    html += "    tr.appendChild(td);";
    html += "    tbody.appendChild(tr);";
    html += "  }";
    html += "}";
    html += "document.addEventListener('DOMContentLoaded', () => {";
    html += "  updateTables();";
    html += "  setInterval(updateTables, 5000);"; // Update tables every 5 seconds
    html += "});";
    html += "function updateCurrentVisitors() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open(\"GET\", \"/visitors\", true);";
    html += "  xhr.onload = function () {";
    html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "      document.getElementById(\"visitors\").textContent = xhr.responseText;";
    html += "    }";
    html += "  };";
    html += "  xhr.send();";
    html += "}";
    // html += "// Call updateCurrentVisitors() every 5 seconds";
    html += "setInterval(updateCurrentVisitors, 5000);";
    // html += "// Call updateCurrentVisitors() initially to populate the value when the page loads";
    html += "updateCurrentVisitors();";
    html += "function updateMaxVisitors() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open(\"GET\", \"/max-visitors\", true);";
    html += "  xhr.onload = function () {";
    html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "      document.getElementById(\"max-visitors\").textContent = xhr.responseText;";
    html += "    }";
    html += "  };";
    html += "  xhr.send();";
    html += "}";

    // html += "// Call updateMaxVisitors() every 5 seconds";
    html += "setInterval(updateMaxVisitors, 1000);";

    // html += "// Call updateMaxVisitors() initially to populate the value when the page loads";
    html += "updateMaxVisitors();";
    html += "</script>";
    html += "</body>";
    html += "</html>";


    _server->on("/max-visitors", HTTP_GET, [&](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(_lock.getMax()));
    });

    _server->on("/visitors", HTTP_GET, [&](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(_rfid.getCount()));
    });



    _server->on("/authorized-tags", HTTP_GET, [&](AsyncWebServerRequest *request) {
        request->send(200, "application/json", _rfid.allowedTagsJSON());
    });

    _server->on("/active-tags", HTTP_GET, [&](AsyncWebServerRequest *request) {
        request->send(200, "application/json", _rfid.existingTagsJSON());
    });

    _server->on("/add-tag", HTTP_GET, [&](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Adding tag, please wait...");
        // xTaskCreate([](void* pvParameters) {
        //   ESPServer* server = static_cast<ESPServer*>(pvParameters);
        //   server->addNewTag();
        //   vTaskDelete(NULL);
        // }, "add-tag", 4096, this, 1, NULL);
        _rfid.updateAccess();
    });

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