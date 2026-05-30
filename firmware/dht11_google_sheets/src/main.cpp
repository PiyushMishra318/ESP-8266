#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>

#include "config.h"

#define DHTTYPE DHT11

DHT dht(kDhtPin, DHTTYPE);

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.printf("Connecting to %s", WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println("\nWiFi connected");
}

bool pushReading(float humidity,
                 float celsius,
                 float fahrenheit,
                 float heatIndexC,
                 float heatIndexF) {
  WiFiClient client;
  if (!client.connect(kPushingBoxHost, kPushingBoxPort)) {
    Serial.println("PushingBox connection failed");
    return false;
  }

  String request = String("GET /pushingbox?devid=") + PUSHINGBOX_DEVID +
                   "&humidityData=" + humidity +
                   "&celData=" + celsius +
                   "&fehrData=" + fahrenheit +
                   "&hicData=" + heatIndexC +
                   "&hifData=" + heatIndexF +
                   " HTTP/1.1\r\n" +
                   "Host: " + kPushingBoxHost + "\r\n" +
                   "User-Agent: ESP8266-DHT11/2.0\r\n" +
                   "Connection: close\r\n\r\n";

  client.print(request);

  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 5000) {
    while (client.available()) {
      client.read();
    }
  }

  client.stop();
  return true;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectWifi();
}

void loop() {
  delay(kSampleIntervalMs);

  const float humidity = dht.readHumidity();
  const float celsius = dht.readTemperature();
  const float fahrenheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit)) {
    Serial.println("Failed to read from DHT sensor");
    return;
  }

  const float heatIndexC = dht.computeHeatIndex(celsius, humidity, false);
  const float heatIndexF = dht.computeHeatIndex(fahrenheit, humidity);

  Serial.printf(
      "Humidity: %.1f%%  Temp: %.1fC / %.1fF  Heat index: %.1fC / %.1fF\n",
      humidity,
      celsius,
      fahrenheit,
      heatIndexC,
      heatIndexF);

  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  if (pushReading(humidity, celsius, fahrenheit, heatIndexC, heatIndexF)) {
    Serial.println("Sent reading to PushingBox");
  }
}
