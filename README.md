# <div align="center">JorgeScript 🚀</div>

<div align="center">
  <img src="https://media3.giphy.com/media/v1.Y2lkPTc5MGI3NjExZTY3aXo0MGpxbHc2aGNvcDNkMWd4cmZyNDd0eTFrZXVpY20ybGM2ZCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/BzDvnHUHdcinvusgn5/giphy.webp" alt="Racing Gif" width=450>

|       |                                                                                                                                                                        |
| ----- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Code  | ![GitHub](https://img.shields.io/badge/License-Proprietary-blue) ![GitHub](https://img.shields.io/badge/Version-1.0-blue) ![GitHub](https://img.shields.io/badge/release-done-orange) ![GitHub](https://img.shields.io/badge/docs-README-white) ![GitHub](https://img.shields.io/badge/Tasks-FreeRTOS-green)|
</div>

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif"><br><br>

# Practice 4: Line Following

This README describes the implementation of Practice 4, aimed at designing a line-following system using Arduino and ESP. Below, you will find details about the components used, the code structure, and the implemented functionalities.

---

## 🔧 System Description

The system is designed to follow a line using an Arduino UNO and communicates with an ESP to manage critical events. It includes:

| **Component**         | **Function**                                                                                   |
|-----------------------|-----------------------------------------------------------------------------------------------|
| Kalman Filter         | Reduces sensor noise and improves precise line detection.                                    |
| PID Controller        | Adjusts motor speeds to keep the vehicle on the line.                                        |
| Obstacle Detection    | Avoids collisions by detecting objects on the vehicle's path.                                |
| Task System           | Divides the code into independent modules for better efficiency and organization.            |
| Indicator LEDs        | Provide real-time visual information about the system's status.                              |
| Communication Protocol| Maintains a system report updated using MQTT                                                |


---

## 🌐 Arduino-ESP Communication

Communication between the Arduino and the ESP is done via the serial port using a structured message protocol. This allows coordination of key system events:

| **Message**                  | **Description**                                                                         |
|-------------------------------|-----------------------------------------------------------------------------------------|
| `<LINE_LOST:0>`              | Indicates the line has been lost.                                                      |
| `<LINE_FOUND:0>`             | Indicates the line has been found.                                                     |
| `<OBSTACLE_DETECTED:dist>`   | Reports an obstacle detected at a distance `dist`.                                     |
| `<VISIBLE_LINE:perc>`        | Indicates the percentage of visible line during a lap.                                 |
| `<END_LAP:0>`                | Marks the end of a lap.                                                                |

For the communication we are using a simple handcrafted protocol:
```python 
"<OBSTACLE_DISTANCE:7>"
```
- **Message structure:**
  1. Before colon is the type of message.
  2. After colon is the value if needed, if not is set to 0.
 
- **Delimiter:**
  1. The message starts with a '<' and ends '>', with this we ensure the message arrived correctly.

### Protocol Implementation

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

MQTT is used to update a topic about what the robot is doing. This can range from starting the lap to losing the line.
For the requirements of the practice, messages are sent as JSON. These JSON structures are provided by the professor as a requirement for the practice. Here's an example:

```json
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

---

## 🔨 Navigation

Below are the three different implemented approaches for line following:

1. **WeightDetector**: This method assigned turn probabilities to each sensor based on the system's global state and perception time. A sigmoid function was then used to translate these weights into control speeds. This approach was very efficient for complex turns (example below), but when the line to follow was too thin, the weights' responsiveness was too slow.
2. **FastDetector**: An optimized, case-based implementation replicating the weight system with manually adjusted values. However, this option was extremely sensitive to noise.
3. **Kalman Filter**: Finally, this approach was chosen for its balance between speed and robustness. Using the data (weights) from the first method, a system was modeled to track the line and dynamically update using Kalman.

### 3. **Kalman Filter**:

The Kalman filter was chosen for its balance between speed and robustness. This method leverages data obtained from **ITR20001** sensors connected to analog pins A0, A1, and A2, modeling a system that continuously predicts and updates the line’s position. The implementation follows these key steps:

#### **Prediction and Update**
1. **Prediction**: Using the system model, the Kalman filter estimates the next state of the line (i.e., its expected position). This calculation relies solely on previous states.

2. **Update**: When new sensor readings arrive, they are compared to the prediction. The filter adjusts its current estimate based on the discrepancy between the expected and measured values. This adjustment includes a gain factor that prioritizes reliable readings over noise.

```cpp

// Convert sensor detection to weights (FSM methodology)
float measurement = detector.measure(); 

// Predict the next state
detector.predict();

// Compare the prediction to the detection
detector.update(measurement);

// Get a new estimation
float position_estimate = detector.getEstimate();
```

#### **Behavior During Line Loss**
When the line is completely lost (no sensor detects the line), the Kalman filter does not interfere. Instead, control shifts to the **PID controller**, where the integrator takes full action. This allows the system to recover the line quickly using accumulated control values. The Kalman filter is designed to "step back" in these moments, avoiding additional noise or overcomplications during the correction process.

> Note: This methodology has been accomplished by adapting an online tutorial in [this page](https://www.robotsforroboticists.com/kalman-filtering/).
---

## 🚗 Dynamic PID Control System

We realized that line detection could not maintain a uniform PID for all scenarios. Thus, we use laser detection to determine when the vehicle is exiting a curve—when no sensor detects the line. In such cases, the system increases aggressiveness to react effectively, returning to a smooth PID when the line is detected again.

1. **Linear Mode (smooth)**: Used when the line is stably detected by the sensors.
2. **Aggressive Mode**: Activated when no sensor detects the line (indicating curve exit). This mode increases the Kp value and activates **drifting**.

## ⚡ Drifting

In aggressive mode, drifting allows one of the motors to take negative speeds, facilitating sharp turns and quick course corrections. The PID integrator plays a crucial role here, providing the appropriate power for these adjustments and ensuring drifting is efficient and controlled. The combined use of the integrator and negative speeds enables the robot to react quickly to sharp curves or extreme deviations, maximizing both style and movement effectiveness.

```cpp
if (detector.lost()) {
  minSpeed(-255); // Drifting activated
  pid.setKp(AGGRESIVE_KP);
  pid.setKi(0.001);
} else {
  minSpeed(0);
  pid.setKp(LINEAR_KP);
  pid.setKi(0.0);
}
```

## 📊 Task System with SimpleRT

The **SimpleRT** library acts as a wrapper over FreeRTOS, simplifying the creation and management of periodic tasks in the system. Its naming and usage are inspired by asynchronous Python libraries like asyncio, enabling developers to transfer prior knowledge to a microcontroller environment. Its purpose is to make the code more readable and maintainable while leveraging FreeRTOS's multitasking capabilities.

### Basic Usage of SimpleRT

- **Initialize a task**: Allows defining tasks with specific priorities.
  
```cpp
SimpleRT::newTask("taskName", taskFunction, priority);
```

- **Wait for the next iteration of a periodic task**: Ensures the system can handle periodic operations seamlessly.

```cpp
SimpleRT rt(20); // Tarea periódica cada 20 ms
rt.awaitNextIteration();
```

- **Pause execution for a defined time**: Temporarily halts task execution to synchronize with the system's timing needs.
  
```cpp
  rt.await(100); // Pausa la ejecución durante 100 ms
```

## ⏱️ Measuring Task Frequencies

The system design ensures that all tasks meet their deadlines in a real-time environment. The most critical task is navigation, with a period of **10 ms**, whose compliance is vital for the robot's functioning. To ensure the RT system operates effectively, task timing has been measured using our custom `NonBlockingTimer`. This library is used to measure the exact time each function takes to execute. It verifies if the consumed time stays within the task's period limit.

Steps followed for this:

1. **Task Measurement**: Time estimates for each task were obtained by running each function multiple times and taking the highest elapsed time plus a margin of error.

```cpp
NonBlockingTimer timer;
timer.start();

// Código de la tarea
work()

unsigned long elapsed = timer.getMsElapsedTime();
Serial.println(elapsed);
```

2. **Deadlines in Critical Functions**: Functions like obstacle detection include time limits. If the function exceeds the allowed time, it is interrupted to avoid system delays.

### Advantages

- **Maximum Frequency**: Ensures that critical tasks are not delayed by less important functions.
- **Robustness**: Deadline control ensures the system remains stable even under unexpected loads.
- **Predictable Times**: Allows analyzing the temporal cost of each task and adjusting periods if necessary.

## 🎥 Results

<div align="center">
  <h3>WeighDetector Tracking</h3>
</div>

<div align="center">
  <video src="https://github.com/user-attachments/assets/8510b777-f50c-4221-9b84-02a3c6fea754" height="200" />
</div>
	  
<div align="center">
  <h3>(Final) Kalman Tracking on Custom Track</h3>
</div>

<div align="center">
  <video src="https://github.com/user-attachments/assets/6f437f8a-2796-4e94-89d8-7aca0afc6e48" height="200" />
</div>

<div align="center">
  <h3>(Final) Kalman on Exam Track</h3>
</div>

<div align="center">
  <video src="https://github.com/user-attachments/assets/ec319da2-20a1-4b47-b527-0dbce1e16eab" height="200" />
</div>
