#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define LED_PIN 4
#define RELAY_PIN 5

#ifndef STASSID
#define STASSID "<SSID>"
#define STAPSK "<PASSWORD>"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

String s;  // Web page content
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", s);
}

void handleNotFound() {
  digitalWrite(LED_PIN, 1);
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
  digitalWrite(LED_PIN, 0);
}

void handleLedOn() {
  digitalWrite(LED_PIN, 1);
  redirect_home();
}

void handleLedOff() {
  digitalWrite(LED_PIN, 0);
  redirect_home();
}

void handleRelayOn() {
  digitalWrite(RELAY_PIN, 1);
  redirect_home();
}

void handleRelayOff() {
  digitalWrite(RELAY_PIN, 0);
  redirect_home();
}

void redirect_home() {
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup(void) {
  // Start serial
  Serial.begin(115200);

  // Prepare GPIO4 (LED)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 0);

  // Prepare GPIO5 (relay)
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, 0);

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

  s = "<!DOCTYPE html>";
  s += "<html>";
  s += "<head>";
  s += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  s += "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN' crossorigin='anonymou'>";
  s += "</head>";
  s += "<body>";
  s += "<div class='container'>";
  s += "<h1>LED</h1>";
  s += "<div class='row g-2 mb-3'>";
  s += "<div class='col-12 col-md-6 col-lg-4'>";
  s += "<a class='btn btn-primary w-100' href='/LedOn'>ON</a>";
  s += "</div>";
  s += "<div class='col-12 col-md-6 col-lg-4'>";
  s += "<a class='btn btn-danger w-100' href='/LedOff'>OFF</a>";
  s += "</div>";
  s += "</div>";
  s += "<h1>Relay</h1>";
  s += "<div class='row g-2'>";
  s += "<div class='col-12 col-md-6 col-lg-4'>";
  s += "<a class='btn btn-primary w-100' href='/RelayOn'>ON</a>";
  s += "</div>";
  s += "<div class='col-12 col-md-6 col-lg-4'>";
  s += "<a class='btn btn-danger w-100' href='/RelayOff'>OFF</a>";
  s += "</div>";
  s += "</div>";
  s += "</div>";
  s += "</body>";
  s += "</html>";

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  // Handle turning the LED on / off
  server.on("/LedOn", handleLedOn);
  server.on("/LedOff", handleLedOff);

  // Handle turning the relay on / off
  server.on("/RelayOn", handleRelayOn);
  server.on("/RelayOff", handleRelayOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
