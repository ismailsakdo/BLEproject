#include <WiFi.h>
#include <HTTPClient.h>
#include "BluetoothSerial.h"

#define SSID "XXXXX"
#define PASSWORD "XXXXXX"
#define THINGSPEAK_API_KEY "XXXXXX"

const char* deviceNames[] = {"ESP A", "ESP B", "ESP C"}; // Device names

BluetoothSerial SerialBT;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32-Client");
  connectToWiFi();
}

void loop() {
  if (SerialBT.available()) {
    String data = SerialBT.readStringUntil('\n');
    int spaceIndex = data.indexOf(' ');
    if (spaceIndex != -1) {
      int rssi = data.substring(0, spaceIndex).toInt();
      String deviceName = data.substring(spaceIndex + 1);
      int numericDeviceName = getNumericDeviceName(deviceName);
      if (numericDeviceName != 0) {
        sendToThingSpeak(rssi, numericDeviceName);
      } else {
        Serial.println("Unknown device name: " + deviceName);
      }
    } else {
      Serial.println("Invalid data received: " + data);
    }
  }
}

void connectToWiFi() {
  WiFi.begin(SSID, PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Failed to connect to WiFi");
  }
}

int getNumericDeviceName(String name) {
  for (int i = 0; i < sizeof(deviceNames) / sizeof(deviceNames[0]); i++) {
    if (name == deviceNames[i]) return i + 1;
  }
  return 0;
}

void sendToThingSpeak(int rssi, int numericDeviceName) {
  if (client.connect("api.thingspeak.com", 80)) {
    String url = "/update?api_key=" + String(THINGSPEAK_API_KEY) +
                 "&field1=" + String(rssi) +
                 "&field2=" + String(numericDeviceName);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: api.thingspeak.com\r\n" +
                 "Connection: close\r\n\r\n");
    client.stop();
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.println("Error connecting to ThingSpeak");
  }
}
