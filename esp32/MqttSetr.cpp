//ESP32
#include "MqttSetr.hpp"


MqttSetr::MqttSetr(char* server, int port, char* username, char* password)
    : mqtt_client_(&wifi_client_, server, port, username, password),
      publisher_(&mqtt_client_, MQTT_TOPIC),
      subscriber_(&mqtt_client_, MQTT_TOPIC) {
    mqtt_client_.subscribe(&subscriber_);
}

void MqttSetr::reconnect() {
  while (!mqtt_client_.connected()) {
    Serial.print("Intentando conectar al broker MQTT...");
    if (mqtt_client_.connect()) {
      Serial.println("¡Conectado!");
      // Resuscribirse a los topics necesarios después de reconectar
      mqtt_client_.subscribe(&subscriber_);
    } else {
      Serial.println("Error al conectar. Reintentando en 5 segundos...");
      delay(5000); // Espera antes de intentar reconectar
    }
  }
}

bool MqttSetr::publish(char* buffer) {
  return publisher_.publish(buffer);
}

void MqttSetr::recive() {

}


bool MqttSetr::ready() {
  if (mqtt_client_.connected()) {
    mqtt_client_.processPackets(10);
    mqtt_client_.ping();
    return true;
  }
  return false;
}