#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "DHT.h"

#define DELAY_IN_BETWEEN_MS 5000

#define DHT_PIN 2
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);

  Serial.println("Serial up and running");
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("<SSID>", "<PASSWORD>");

  dht.begin();
  Serial.println("DHT11 up and running");

  Serial.println("Starting, please wait...");
  delay(5000);
}

void loop() {
  // One new connection for each upload
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    // Read temperature and return on failure
    float t = dht.readTemperature();
    if (isnan(t)) {
      Serial.println("Reading from DHT sensor failed");
      return;
    }
    float h = dht.readHumidity();

    // Display temperature and relative humidity
    Serial.printf("[SENSOR] Temperature = %f*C\n", t);
    Serial.printf("[SENSOR] Humidity = %f\%\n", h);

    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://api.thingspeak.com/update?api_key=<API_KEY>&field1="+String(t)+"&field2="+String(h))) {  // HTTP
      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println("Number of items on channel = " + payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP] Unable to connect\n");
    }
  }
  delay(DELAY_IN_BETWEEN_MS);
}
