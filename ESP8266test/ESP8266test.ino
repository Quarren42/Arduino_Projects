
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define OutputPin 2

#define WLAN_SSID       "searching..."
#define WLAN_PASS       "12345678"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "quarren42"
#define AIO_KEY         "c48b8b362e2c473f969237870616c92c"

void connect();

WiFiClient client;

/*const char MQTT_SERVER[] PROGMEM = AIO_SERVER;
  const char MQTT_CLIENTID[] PROGMEM  = AIO_KEY __DATE__ __TIME__;
  const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
  const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;
*/

//Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//const char lightFeed[] PROGMEM = AIO_USERNAME "/feeds/test-feed";
Adafruit_MQTT_Subscribe light = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/test-feed");

void setup() {
  pinMode(OutputPin, OUTPUT);
  Serial.begin(115200);

  Serial.println(F("Adafruit IO Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  mqtt.subscribe(&light);

  connect();
}

void loop() {
  //if (! mqtt.ping(3)) { //check if connected to IO
    // reconnect to adafruit io
    if (! mqtt.connected())
      connect();
 // }
  
  Adafruit_MQTT_Subscribe *subscription;
  while (subscription = mqtt.readSubscription(1000)) {

    // we only care about the lamp events
    if (subscription == &light) {

      // convert mqtt ascii payload to int
      char *value = (char *)light.lastread;
      Serial.print(F("Received: "));
      Serial.println(value);

      // Apply message to lamp
      String message = String(value);
      message.trim();
      if (message == "on") {
        digitalWrite(OutputPin, LOW);
      }
      if (message == "off") {
        digitalWrite(OutputPin, HIGH);
      }
    }
  }
}

void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }
  Serial.println(F("Adafruit IO Connected!"));
}

