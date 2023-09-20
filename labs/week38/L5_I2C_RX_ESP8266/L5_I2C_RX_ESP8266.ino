/*
 * This is a stripped-down version of the HelloServer sketch.
 * It reads a string from I2C and shows this as
 * plain text on a web page.
 */
#include <Wire.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "NotFreeWiFi"
#define STAPSK  "dcdb3739"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

String s = "";
ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  server.send(200, "text/plain", s);
}

void handleNotFound() {
  server.send(404, "text/plain", "Error");
}

void setup(void) {
  Wire.begin(D4,D5);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
 delay(1000);
 s= "";
 Wire.beginTransmission(0x10);
 Wire.write("Hello Arduino");
 Wire.endTransmission();
 Wire.requestFrom(0x10, 12);
 while(Wire.available()){
    s = s + char(Wire.read());
 }
 s = s.substring(0,11);
 server.handleClient();
  MDNS.update();
}
