#include <WiFi.h>

const char* ssid = "Ybenel";
const char* password = "benybenel10";

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    int connectionAttempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        if (connectionAttempts >= 10) {
            Serial.println("\nFailed to connect to WiFi network");
            break;
        }
        Serial.print(".");
        delay(500);
        connectionAttempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to the WiFi network");
        Serial.print("Local ESP32 IP: ");
        Serial.println(WiFi.localIP());
    }
}

void loop() {
    
}
