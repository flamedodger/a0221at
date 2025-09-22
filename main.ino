#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "Your WIFI SSID";
const char* password = "Your WIFI password";

// MQTT broker settings
const char* mqtt_server = "192.168.0.52";  // Your Home Assistant IP
const char* mqtt_user = "mqtt_user";       // Your MQTT username
const char* mqtt_password = "57NpzeZf!egm3ju";  // Your MQTT password
const char* mqtt_topic = "home/ao221at/distance";

WiFiClient espClient;
PubSubClient client(espClient);

// UART pins
#define RXD2 16
#define TXD2 17

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? "\nWi-Fi connected!" : "\nWi-Fi connection failed.");
}

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32_AO221AT", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect_mqtt();
  client.loop();

  Serial2.write(0x55);
  delay(100);

  if (Serial2.available() >= 4) {
    uint8_t buffer[4];
    Serial2.readBytes(buffer, 4);

    if (buffer[0] == 0xFF) {
      int distance = (buffer[1] << 8) | buffer[2];
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" mm");

      char payload[16];
      snprintf(payload, sizeof(payload), "%d", distance);
      client.publish(mqtt_topic, payload);
    } else {
      Serial.print("Bad frame: ");
      for (int i = 0; i < 4; i++) {
        Serial.printf("0x%02X ", buffer[i]);
      }
      Serial.println();
    }
  } else {
    Serial.println("No response from sensor");
  }

  delay(1000);
}
