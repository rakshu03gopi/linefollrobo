# linefollrobo
# 🤖 Autonomous Line Following Delivery Robot

An Arduino-based autonomous line-following robot capable of navigating to user-selected destinations, detecting nodes, performing predefined turns, and automatically returning to the starting point.

## 🚀 Features

- Line following using 3 IR sensors
- Serial command-based destination selection (Nodes 1–8)
- Automatic node detection
- Predefined left turns at selected junctions
- Automatic 180° return after reaching destination
- Buzzer feedback for important events
- Simple state-machine based navigation

---

## 🛠️ Hardware Used

- Arduino Uno
- L298N Motor Driver
- 2 DC Geared Motors
- 3 IR Line Sensors
- Buzzer
- Chassis with Wheels
- Battery Pack

---

## 📌 Working

1. Upload the Arduino code.
2. Open the Serial Monitor (9600 baud).
3. Send a destination node (1–8).
4. The robot follows the line and counts junctions.
5. At predefined nodes, it performs left turns.
6. On reaching the destination, it stops and performs a 180° turn.
7. The robot follows the same path back to the starting point.

---

## ⚙️ Navigation Logic

- **Node Detection:** All three sensors detect black (`111`).
- **Forward Motion:** Center sensor follows the line.
- **Turns:** Configurable using the `leftTurnNode[]` array.
- **Return Mode:** Automatically activated after reaching the destination.
- **Buzzer:** Indicates command received, destination reached, and return completion.

---

## 📂 Pin Configuration

| Component | Pin |
|-----------|-----|
| Left Sensor | A0 |
| Middle Sensor | A1 |
| Right Sensor | A2 |
| Left Motor EN | 6 |
| Right Motor EN | 11 |
| Left Motor IN1 | 10 |
| Left Motor IN2 | 9 |
| Right Motor IN1 | 8 |
| Right Motor IN2 | 7 |
| Buzzer | 12 |

---

## 💻 Technologies

- Arduino C/C++
- Embedded Systems
- Motor Control
- Line Following
- State Machine Logic

---

## 📈 Future Improvements

- PID-based line following
- Obstacle detection using Ultrasonic Sensor
- RFID/QR-based node identification
- Wireless destination selection via Bluetooth/Wi-Fi
- Dynamic path planning

---

## 📜 License

This project is intended for educational and research purposes.
