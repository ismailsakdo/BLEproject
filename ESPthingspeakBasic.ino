#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

const char* ssid = "XXXXX";
const char* password = "XXXXX";
const char* thingSpeakApiKey = "XXXXX";
const unsigned long thingSpeakChannel = XXXXX;

// Define unique identifier for each ESP32
const char* deviceName = "Ali"; // Change this for each ESP32
const int deviceNumber = 2;      // Change this for each ESP32

WiFiClient client;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    ThingSpeak.begin(client);
}

void loop() {
    int rssi = WiFi.RSSI();

    if (rssi > -70) { // Adjust threshold as needed
        sendData(deviceNumber, rssi);
    }

    delay(10000); // Scan every 10 seconds
}

void sendData(int deviceNumber, int rssi) {
    Serial.print("Sending data from ");
    Serial.println(deviceName);
    Serial.print("RSSI: ");
    Serial.println(rssi);

    ThingSpeak.setField(1, deviceNumber);
    ThingSpeak.setField(2, rssi);

    int status = ThingSpeak.writeFields(thingSpeakChannel, thingSpeakApiKey);
    if (status == 200) {
        Serial.println("Data sent successfully");
    } else {
        Serial.println("Failed to send data. Error code: " + String(status));
    }

    delay(15000); // Wait 15 seconds before sending next data
}
