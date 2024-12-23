//ESP32
#include "MqttSetr.hpp"


// Class builder
MqttSetr::MqttSetr(char* server, int port, char* username, char* password)
    : mqtt_client_(&wifi_client_, server, port, username, password),
      publisher_(&mqtt_client_, MQTT_TOPIC),
      subscriber_(&mqtt_client_, MQTT_TOPIC) {
    mqtt_client_.subscribe(&subscriber_);
}

// Reconnect function
void MqttSetr::reconnect() {
  // While is not connected
  while (!mqtt_client_.connected()) {
    // Traze
    Serial.print("Intentando conectar al broker MQTT...");
    // If it is connected
    if (mqtt_client_.connect()) {
      Serial.println("¡Conectado!");
      // Resuscribes to topics
      mqtt_client_.subscribe(&subscriber_);
    }
  }
}

// Function to publish
bool MqttSetr::publish(char* buffer) {
  // Publish and returns true or false
  return publisher_.publish(buffer);
}

// Function not needed at the end
void MqttSetr::recive() {

}

// Checks if mqtt is connected
bool MqttSetr::ready() {
  // Checks if it is connected and decides starting params
  if (mqtt_client_.connected()) {
    mqtt_client_.processPackets(10);
    mqtt_client_.ping();
    return true;
  }
  return false;
}