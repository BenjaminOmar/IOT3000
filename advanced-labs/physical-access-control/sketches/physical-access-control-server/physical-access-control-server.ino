#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>

#define ADDRESS_ARDUINO 8

#define SECRET_SSID "<SSID>"
#define SECRET_PASS "<PASS>"
#define TIMEOUT_MS  3000

#define CODE_LENGTH   4
#define DEVICE_ID     1

const char *SSID = SECRET_SSID;
const char *PASS = SECRET_PASS;

const int PORT = 5000;
const String HOST_NAME = "127.0.0.1";
const String BASE_URL = "http://" + HOST_NAME + ":" + String(PORT) + "/";
const String API_URL = BASE_URL + "api/v1/";

WiFiClient wifiClient;
HTTPClient httpClient;

String userToken = "";

void setup() {
  Serial.begin(9600);

  Serial.print("[SETUP] [WIFI] Connecting to \"" + String(SSID) + "\"");
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(TIMEOUT_MS);
  } Serial.println();
  Serial.println("[SETUP] [WIFI] Connection successful");
  Serial.println("[SETUP] [WIFI] [IP] " + WiFi.localIP().toString());

  Serial.printf("[SETUP] [CLIENT] Connecting to \"%s\"", BASE_URL);
  while ( !wifiClient.connect(HOST_NAME, PORT) ) {
    Serial.print(".A.");
    delay(TIMEOUT_MS);
  } Serial.println();
  Serial.println("[SETUP] [CLIENT] Connection successful");

  Wire.begin(ADDRESS_ARDUINO);
  Wire.onReceive(receiveEvent);
  Serial.println("[SETUP] [WIRE] Ready to communicate with Arduino");

  delay(2000);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) return;

  if (userToken.length() == 0) {
    userToken = fetchUserToken();
    if (userToken.length() > 0) {
      Serial.println("[TOKEN] " + userToken);
    }
  }

  delay(5000);
}

void receiveEvent(int bytes) {
  String code = "";
  while (0 < Wire.available()) {
    char c = Wire.read();
    code += c;
  }
  if (code.length() == CODE_LENGTH) {
    sendCodeAttemptRequest(code);
  }
}

String fetchUserToken() {
  Serial.println("[HTTP] Fetching user token...");

  httpClient.begin(wifiClient, API_URL + "/auth/login");
  httpClient.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> doc;
  doc["email"] = "admin@accesscontrolsystem.com";
  doc["password"] = "accesscontrolsystem123?";

  String requestBody;
  serializeJson(doc, requestBody);

  int httpResponseCode = httpClient.POST(requestBody);
  if (httpResponseCode > 0) {
    String response = httpClient.getString();
    Serial.println(response);

    StaticJsonDocument<768> doc;
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
      Serial.print("JSON deserialization failed: ");
      Serial.println(error.f_str());
    } else {
      return doc["token"]; 
    }
  } else Serial.printf("[HTTP] An error occurred: %s", httpClient.errorToString(httpResponseCode).c_str());
  
  return "";
}

void sendCodeAttemptRequest(String code) {
  Serial.println("[HTTP] Fetching user token...");
  
  httpClient.begin(wifiClient, API_URL + "/attempt");
  httpClient.addHeader("Content-Type", "application/json");
  httpClient.addHeader("Authorization", "Token " + userToken);

  StaticJsonDocument<200> doc;
  doc["code"] = code;
  doc["device"] = DEVICE_ID;

  String requestBody;
  serializeJson(doc, requestBody);

  int httpResponseCode = httpClient.POST(requestBody);
  if (httpResponseCode > 0) {
    String response = httpClient.getString();
    Serial.println(response);

    StaticJsonDocument<768> doc;
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
      Serial.print("JSON deserialization failed: ");
      Serial.println(error.f_str());
    }
  } else Serial.printf("[HTTP] An error occurred: %s\n", httpClient.errorToString(httpResponseCode).c_str());
}