#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
    String html = "<html><head><title>ESP32 Web Server</title>";

    server.send(200, "text/html", html);
}

void generateWebServer() {
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}