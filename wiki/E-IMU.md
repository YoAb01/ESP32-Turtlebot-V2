# 📁 E - IMU


### **Introduction**
---
IMU stands for Inertial Measurement Unit. It is a sensor module that provides:

* Acceleration data (3 axes)
* Angular velocity (gyroscope) data (3 axes)
* Often temperature data

This information is crucial for:

* Estimating robot orientation and inclination.
* Detecting motion, rotation, and impacts.
* Implementing control loops (e.g. balancing robots, drones).


### What is an MPU6050?
---
The MPU6050 is a popular low-cost 6-DoF IMU module. It combines 3-axis accel, 3-axis gyro, temperature data dn I2C communication interface.

__Note__: _An IMU is like the robot’s sense of balance and movement – just as our inner ear tells us if we’re moving forward, tilting, or spinning._


### ESP32 I2C communication with MPU6050
---
The ESP32 communicates with MPU6050 using I2C protocol:

1. I2C bus basics:
* Two wires: SDA (data) and SCL (clock).
* Multiple devices can share the same bus with unique addresses.

2. MPU6050 address: 0x68 (default according to the documentation).

3. Basic workflow:
* Initialize I2C master peripheral.
* Wake up the MPU6050 by writing 0 to the power management register.
* Read data registers to obtain raw accelerometer and gyroscope data.


### Class design
---
The IMU class encapsulates MPU6050 initialization and data reading:

* `Constructor`: Takes sda_pin and sck_pin to define I2C pin configuration.

* `init_imu()`:
    * Configures ESP32 I2C master.
    * Wakes up the MPU6050 from sleep mode by writing 0 to register 0x6B.

* `read_data()`:
    * Reads raw sensor data from register 0x3B.
    * Processes and converts to:
        * Accelerometer data: accelX, accelY, accelZ (m/s^2)
        * Gyroscope data: gyroX, gyroY, gyroZ (°/s)
        * Temperature data: tempC (°C)

### Application
---
In main, we can define a task that manages to init and read data from it:
1. Create IMU instance: `IMU imu(GPIO_NUM_21, GPIO_NUM_22);`
2. Initialize MPU6050: `imu.init_imu();`
3. Read data in loop.

### Final Summary
---
💡 **_Think of the IMU class as the robot’s vestibular system – giving it a sense of acceleration, tilt, and rotation to navigate and balance effectively._**