#include "MqttSetr.hpp"
#include <WiFi.h>
#include "passwords.hpp"
#include <ArduinoJson.h>
#include <Thread.h>
#include <StaticThreadController.h>
#include <ThreadController.h>

#define MQTT_SERVER "teachinghub.eif.urjc.es"  // Dirección del broker
#define MQTT_SERVERPORT 21883                  // Puerto MQTT

#define RXD2 33
#define TXD2 4

MqttSetr mqtt_prove = MqttSetr(MQTT_SERVER, MQTT_SERVERPORT, "", "");

// char* ssid = "POCO F3";
// char* password = "12341234";

// Configuración Wi-Fi
#define EAP_ANONYMOUS_IDENTITY "20220719anonymous@urjc.es"  // leave as it is
#define EAP_IDENTITY URJC_EMAIL                             // Use your URJC email
#define EAP_PASSWORD URJC_PASS                              // User your URJC password
#define EAP_USERNAME URJC_EMAIL                             // Use your URJC email
char* ssid = "eduroam";

long unsigned start_time;

ThreadController controller = ThreadController();
Thread pingThread = Thread();

char json_buffer[200];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
  delay(10);

  // Conectar a Wi-Fi
  Serial.print("Conectando a Wi-Fi...");
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
  // WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }

  Serial.println("¡Conectado!");

  while (!mqtt_prove.ready()) {
    mqtt_prove.reconnect();
  }

  pingThread.enabled = true;
  pingThread.setInterval(4000);
  create_json("START_LAP", 0);
  mqtt_prove.publish(json_buffer);
  Serial2.println("<Arduino Start>");
  pingThread.onRun(pingSender);
  controller.add(&pingThread);
  start_time = millis();
}

void loop() {
  controller.run();
  String action;
  int timer;
  // Leer datos del Arduino
  if (Serial2.available()) {
    String recibido = Serial2.readStringUntil('\n');  // Leer hasta nueva línea
    recibido.trim();

    if (validarMensaje(recibido)) {
      int index = recibido.indexOf(":");

      action = recibido.substring(1, index);
      timer = recibido.substring(index + 1, recibido.length() - 1).toInt();

      create_json(action, timer);
      mqtt_prove.publish(json_buffer);
    }
  }
}

bool validarMensaje(String mensaje) {
  // Verificar delimitadores
  return (mensaje.startsWith("<") && mensaje.endsWith(">"));
}

void pingSender() {
  unsigned long ping_time = millis() - start_time;
  create_json("PING", ping_time);
  mqtt_prove.publish(json_buffer);
}

void create_json(String function, unsigned long value) {
  StaticJsonDocument<200> jsonDoc;

  if (function == "PING") {
    jsonDoc["team_name"] = "JorgeScript";
    jsonDoc["id"] = "6";
    jsonDoc["action"] = function;
    jsonDoc["time"] = millis() - start_time;
  }
  if (function == "END_LAP") {
    jsonDoc["team_name"] = "JorgeScript";
    jsonDoc["id"] = "6";
    jsonDoc["action"] = function;
    jsonDoc["time"] = (millis() - start_time);
    pingThread.enabled = false;
  }
  if (function == "VISIBLE_LINE") {
    jsonDoc["team_name"] = "JorgeScript";
    jsonDoc["id"] = "6";
    jsonDoc["action"] = function;
    jsonDoc["value"] = value;
  }
  if (function == "OBSTACLE_DETECTED") {
    jsonDoc["team_name"] = "JorgeScript";
    jsonDoc["id"] = "6";
    jsonDoc["action"] = function;
    jsonDoc["distance"] = value;
  } else {
    jsonDoc["team_name"] = "JorgeScript";
    jsonDoc["id"] = "6";
    jsonDoc["action"] = function;
  }

  serializeJson(jsonDoc, json_buffer);
}
