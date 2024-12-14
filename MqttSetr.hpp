#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <WiFi.h>

#define MQTT_TOPIC "/SETR/2024/6/"

class MqttSetr {
public:
  MqttSetr(char*, int, char*, char*);
  void reconnect();
  bool publish(char*);
  void recive();
  bool ready();

private:
  Adafruit_MQTT_Publish publisher_;
  Adafruit_MQTT_Subscribe subscriber_;
  Adafruit_MQTT_Client mqtt_client_;
  WiFiClient wifi_client_;
};