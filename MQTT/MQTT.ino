#include <WiFi.h>
#include <PubSubClient.h>

#define SSID                    "microlab_IoT"
#define WIFI_PWD                "shibboleet"

#define MQTT_BROKER             "test.mosquitto.org"
#define MQTT_PORT               (1883)

#define MQTT_PUBLIC_TOPIC       "uok/iot/<YOUR LOGIN>/capacitive"

#define TOUCH_PIN               (4)

WiFiClient wifiClient;
PubSubClient client(wifiClient);

boolean touchFlag = false;

void setup() {
Serial.begin(115200); // Initialise serial messages for debuggin purposes.

  // We start by connecting to a WiFi network
Serial.print("Connecting to " SSID);
WiFi.begin(SSID, WIFI_PWD); // attempt to connect to an existing wifi 
//Wait for wifi to connect
while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print("."); 
}
Serial.println("WiFi connected"); 
Serial.print("IP address: ");
Serial.println(WiFi.localIP());

  // Handle MQTT connection.
  client.setServer(MQTT_BROKER, MQTT_PORT); // set broker settings
  while (!client.connected()) { // check connected status
    if (client.connect(("ESP32-"+String(random(0xffff), HEX)).c_str())) { // connect with random id
      Serial.println("MQTT connected."); // report success 
    } else {
      Serial.printf("failed, rc=%d try again in 5 seconds", client.state()); // report error
      delay(5000); // wait 5 seconds
    }
} }

void loop() {

  // Let the MQTT client manage its internals.
  client.loop();

  if(touchRead(TOUCH_PIN) < 60) { // if you are touching the target pin
    if(!touchFlag) { // and you weren;’t previously,
      touchFlag = true; // mark as currently touching 
      Serial.println("Touched"); // report touching 
      client.publish(MQTT_PUBLIC_TOPIC, "touch"); // publish message
    }
  } else { // otherwise (you are not touching)
    touchFlag = false; // un-mark
  }
}