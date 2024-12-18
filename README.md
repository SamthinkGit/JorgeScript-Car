# JorgeScript-Car

# Communications on Robot car

## Serial
### Explanation
Serial is implemented between arduino and esp to exchange what types of messages you want to send to MQTT. Those messages are , for example, if the robot has lost the line or if he ended with the circuit.

### Protocol

For the communication we are using a simple protocol, it a seems something like this:

"<OBSTACLE_DISTANCE:7>"

- **Message structure:**
  1. Before colon is the type of message.
  2. After colon is the value if needed, if not is set to 0.
 
- **Delimiter:**
  1. The message starts with a '<' and ends '>', with this we ensure the message arrived correctly.

### Implementation

1. **Message sending**
   Arduino sends message through serial port every time it needs to communicate with MQTT.

2. **Message receiving**
   Esp32 loop is always checking for new data, before processing it, the message is checked.

3. **Initialization**
  - Esp32 sends a message when the WiFi and MQTT is ready to work.
  - In Arduino's setup the a clausele which doesn't start until this message arrives.

### Problems

Most of the problems of the serial communication are related to secure the message goes in time and also are taken the right way. For example the int value will corrupt first test of the serial.


## MQTT

### Explanation

MQTT is used to update a topic about what the robot is doing. This can range from starting the lap to losing the line.

### Messages

For the requirements of the practice, messages are sent as JSON. These JSON structures are provided by the professor as a requirement for the practice. Here's an example:

```python
{
	"team_name": "$YOUR_TEAM_NAME",
	"id": "$TEAM_ID",
	"action": "START_LAP"
}
```

### Class

To simplify the use of MQTT, we implemented a small class called MqttSetr. This class includes encapsulated methods for a faster approach. The methods and constructor are as follows:

```c++
MqttSetr(char*, int, char*, char*);  // MqttSetr(ip_of_server , server_port, user, password);
void reconnect();
bool publish(char*);
void recive();
bool ready();
```

### Implementation

The MQTT implementation is only on the ESP32. The complete process to send a message is as follows:

1. Receive the message from serial without processing.
2. Process the message to extract the information.
3. Convert the information into a JSON object.
4. Serialize the JSON object to create a char*.
5. Send the serialized JSON to the pre-established topic.

For creating JSON objects, we used the ArduinoJson [library](https://arduinojson.org) .

### Problems

The main problems with MQTT were related to understanding the [Adafruit MQTT Library](https://docs.arduino.cc/libraries/adafruit-mqtt-library/).
