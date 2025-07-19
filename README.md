# 🐢 ESP32 Turtlebot V2 Project

### 🚀 **Project Description**

This project is a **full-stack embedded robotics platform** built around the **ESP32-S3 WROOM module**, aiming to create a powerful, extensible Turtlebot-like mobile robot from scratch.

---

### 🎯 **Project Goals**

✅ Develop a modular, well-architected ESP32-based robotics codebase

✅ Implement drivers for motors, sensors, and communication modules

✅ Integrate AI/ML features leveraging ESP32-S3’s capabilities

✅ Provide full documentation of ESP32 and related embedded engineering for future reference

---

### 🛠️ **Project Hardware**

- **Microcontroller:** ESP32-S3 WROOM with onboard camera
- **Motors:** 2WD DC motor driver (L298N)
- **Sensors:**
  - MPU6050-6DoF (IMU)
  - GY-273-3DoF (Magnetometer)
  - GY-NEO6MV2 NEO-6M (GPS)
  - Ultrasonic sensor (HC-SR04)
  - IR Obstacle avoidance sensors
  - Line following sensors
  - Wheel encoders for odometry
- **Other:**
  - LEDs, RGB indicators
  - Shift registers (74HC595)
  - WiFi and UDP communication for remote control

---

### 📅 **Roadmap & Milestones**

#### ✅ **Phase 1 – Embedded Foundations**

- [x] **1.1:** GPIO + FreeRTOS LED Task
  - [x] Blink LED at 1Hz using FreeRTOS task and timer interrupt

- [x] **1.2:** Button input reading + interrupt handling

- [x] **1.3:** PWM motor driver control with MCPWM

- [x] **1.4:** I2C communication with MPU6050 (IMU driver)

- [ ] **1.5:** Ultrasonic sensor interfacing with timer-based measurement

- [x] **1.6:** Infrared sensor integration for obstacle detection

- [ ] **1.7:** Servo motor control via PWM for sensor scanning (Optional)

---

#### ✅ **Phase 2 – Sensors, Communication & Control**

- [ ] **2.1:** Camera Integration
  - [x]  Initialize camera driver (esp32-camera)
  - [x]  Capture still image and print confirmation log
  - [ ]  Stream camera feed to local HTTP server (test only)
  - [ ]  Process image data for brightness/obstacle confirmation (Optional)

- [x] **2.2:** WiFi connectivity (Station + Access Point modes)
  - [x] Setup robust WiFi connection in both modes

- [x] **2.3:** UDP-based joystick remote control
  - [x] Receive joystick axes and button states via UDP socket

- [ ] **2.4:** Manual vs Auto mode switching
  - [ ] Toggle between teleoperation (manual) and autonomous modes using joystick buttons

- [ ] **2.5:** Actuator feedback control via joystick buttons
  - [ ] Trigger buzzer, LEDs, or servo based on joystick inputs

- [x] **2.6:** Basic teleoperation control logic
  - [x] Map joystick axes to differential drive motor speeds

---

#### ✅ **Phase 3 – Modular Robotics System & Intelligence**

- [ ] **3.1:** Integration of motor, IMU, ultrasonic, and IR modules

- [ ] **3.2:** Basic navigation and obstacle avoidance logic (autonomous mode)

- [ ] **3.3:** AI/ML inference on ESP32-S3 for gesture recognition or simple CV

- [ ] **3.4:** Train a TinyML algorithm to recognize current motion states using IMU data

- [ ] **3.5:** Web server control interface (ESP-IDF HTTP server + JS/CSS frontend + Python socket.io backend)


---

> **Note:** This README serves as both a **project description** and an **initial roadmap**. It will be updated regularly to reflect progress, new milestones, and learned insights.

