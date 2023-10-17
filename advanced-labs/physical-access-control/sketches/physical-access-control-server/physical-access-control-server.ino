#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "NotFreeWiFi"
#define STAPSK "dcdb3739"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

String s;  // Web page content
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", s);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404, "text/plain", message);
}

void handleGranted() {
  s = "";
  s = "<DOCTYPE html>";
  s += "<html>";
  s += "<body>";
  s += "<h1>Login successful</h1>";
  s += "<p>You are now logged in!</p>";
  s += "</body>";
  s += "</html>";
}

void handleDenied() {
  s = "";
  s = "<DOCTYPE html>";
  s += "<html>";
  s += "<body>";
  s += "<h1>BUMMER</h1>";
  s += "<p>DENIED!</p>";
  s += "</body>";
  s += "</html>";
}

void redirect_home() {
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup(void) {
  // Start serial
  Serial.begin(115200);

  // TODO PINs

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  s = "<DOCTYPE html>";
  s += "<html>";
  s += "<body>";
  s += "<h1>Home</h1>";
  s += "<p>Waiting for login attempt...</p>";
  s += "</body>";
  s += "</html>";

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  // Handle turning the LED on / off
  server.on("/granted", handleGranted);
  server.on("/denied", handleDenied);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
