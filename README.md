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

#### **Phase 1 – Embedded Foundations**

- **1.1:** GPIO + FreeRTOS LED Task
  - Blink LED at 1Hz using FreeRTOS task and timer interrupt 
- **1.2:** Button input reading + interrupt handling
- **1.3:** PWM motor driver control with MCPWM
- **1.4:** I2C communication with MPU6050 (IMU driver)
- **1.5:** Ultrasonic sensor interfacing with timer-based measurement
- **1.6:** Servo motor control via PWM for sensor scanning

---

#### **Phase 2 – Communication & Control**

- **2.1:** WiFi connectivity in Station + Access Point modes
- **2.2:** UDP-based joystick remote control
- **2.3:** Web server control interface (ESP-IDF HTTP server)

---

#### **Phase 3 – Modular Robotics System**

- **3.1:** Integration of motor, IMU, and ultrasonic modules
- **3.2:** Basic navigation and obstacle avoidance logic
- **3.3:** AI/ML inference on ESP32-S3 for gesture recognition or simple CV
- **3.4:** Train a TinyML algorithm to recognize current state motion using IMU data training

---

> **Note:** This README serves as both a **project description** and an **initial roadmap**. It will be updated regularly to reflect progress, new milestones, and learned insights.

