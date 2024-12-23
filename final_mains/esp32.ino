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

// Configuración Wi-Fi
#define EAP_ANONYMOUS_IDENTITY "20220719anonymous@urjc.es"  // leave as it is
#define EAP_IDENTITY URJC_EMAIL                             // Use your URJC email
#define EAP_PASSWORD URJC_PASS                              // User your URJC password
#define EAP_USERNAME URJC_EMAIL                             // Use your URJC email
char* ssid = "eduroam";

long unsigned start_time;

// Creates thread and thread controller for ping
ThreadController controller = ThreadController();
Thread pingThread = Thread();

// Char buffer for sending jsons
char json_buffer[200];

// Setup
void setup() {
  // Initializes both serial
  Serial.begin(115200);
  Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
  delay(10);

  // Connects to wifi of eduroam
  Serial.print("Conectando a Wi-Fi...");
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);


  // Goes checking if wifi is already connected with a bit of delay
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }

  // Shows on Serial monitor if its connected
  Serial.println("¡Conectado!");

  // While it not connected to mqqt retries
  while (!mqtt_prove.ready()) {
    mqtt_prove.reconnect();
  }

  // Once is connected starts thread for ping
  pingThread.enabled = true;
  pingThread.setInterval(4000);

  // Creates json for starting lap
  create_json("START_LAP", 0);

  //Publishes json
  mqtt_prove.publish(json_buffer);
  
  // Sends message to start on Arduino
  Serial2.println("<Arduino Start>");

  // Starts thread
  pingThread.onRun(pingSender);

  // Adds thread
  controller.add(&pingThread);
  
  // Take starting millis
  start_time = millis();
}

// Loop
void loop() {
  // Runs controller
  controller.run();

  // Variables 
  String action;
  int timer;

  // Leer datos del Arduino
  if (Serial2.available()) {
    String recibido = Serial2.readStringUntil('\n');  // Leer hasta nueva línea
    recibido.trim();

    // If meesage is correct
    if (validarMensaje(recibido)) {
      // Takes index of : char
      int index = recibido.indexOf(":");

      // Splits the message into two
      action = recibido.substring(1, index);
      timer = recibido.substring(index + 1, recibido.length() - 1).toInt();

      // Creates json with header and values
      create_json(action, timer);

      // Publishes json
      mqtt_prove.publish(json_buffer);
    }
  }
}


// Function to validate messages
bool validarMensaje(String mensaje) {
  // Checks if starts and ends with delimiters
  return (mensaje.startsWith("<") && mensaje.endsWith(">"));
}

// Function to send ping
void pingSender() {
  // Takes time since start of lap
  unsigned long ping_time = millis() - start_time;

  // Creates message
  create_json("PING", ping_time);
  
  // Publishes messages
  mqtt_prove.publish(json_buffer);
}

// Function to create the json
void create_json(String function, unsigned long value) {
  StaticJsonDocument<200> jsonDoc;

  // Checks the header and create the json accordingly
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

  // Transforms json into char*
  serializeJson(jsonDoc, json_buffer);
}
