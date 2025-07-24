# 📁 D - Motor

### **Introduction**
---
Motors are the fundamental actuators of any mobile robot. They convert electrical energy into mechanical motion to move wheels, tracks, or perform other actions. In embedded robotics systems, DC motors are widely used for driving because of their simplicity and direct speed control via voltage or PWM.

In our TB2, we will be using the famous TT motor since it is largely used among simple robotics application, and fairly enough for a small mobile robot like our. 

### What is a DC motor driver?
---
A motor driver circuit or module is an electronic interface between microcontrollers (which cannot supply high current) and motors (which require significant current and sometimes bidirectional drive). It typically uses an H-bridge circuit to:

- Control the direction of rotation (forward/reverse)
- Control the speed by varying PWM duty cycle

In our case we will be using the L298N module. It is a dual H-bridge driver capable of driving two DC motors bidirectionally.

### Motor control basics
To control a DC motor:

1. Set direction. Using 2 digital outputs (e.g. IN1 and IN2) connected to the driver.
    - IN1: HIGH and IN2: LOW  --->  Forward
    - IN1: LOW and IN2: HIGH  --->  Reverse
    - Both LOW or HIGH        --->  Stop

2. Set speed. Using PWM output on the enable pin we can set a duty cycle (%) which determines the voltage average (and hence the motor speed)

__Note__: _Controlling a motor is like pressing your car's accelerator (PWM speed) while deciding whether to go forward or reverse with the gear shift (direction pins)._

### ESP32 motor control with MCPWM
---
On ESP32-S3, motors are commonly controlled via the MCPWM peripheral (Motor Control PWM) for precise PWM generation. The workflow is:

1. Initialize MCPWM unit and timer for PWM output.
2. Configure GPIO pins as MCPWM outputs.
3. Set duty cycle to control speed.
4. Configure direction pins as GPIO outputs and set them HIGH or LOW depending on desired direction.

### Class design
---
Using this knowledge, the Motor class abstracts motor control into:
* `Contructor`: Initializes the motor pins (IN1, IN2) and PWM channel (PWM_PIN ENA or ENB)
* `init()`:  Configures GPIO pins and PWM channel 
    * Configure the pin direction `gpio_config_t` 
    * Configure the PWM pin with `mcpwm_gpio_init`
    * Configure PWM settings with `mcpwm_config_t` and initialize it with `mcpwm_init`
* `motor_control`: 
    * Accept a float input which is the percent of duty cycle (-100% to +100%)
    * Negative → reverse, positive → forward, 0 → stop.
    * Internally, we set the pins direction accordinly to the duty cycle and we apply the value as PWM duty cycle to control speed with `mcpwm_set_duty`

### Application
---
We can use this Motor class to control a differential drive robot. So in `main.cpp`

1. Create two motor instances:
* `Motor left_motor(pwmA, dirA1, dirA2);`
* `Motor left_motor(pwmA, dirA1, dirA2);`

2. Initialize them:
* `left_motor.init();`
* `left_motor.init();`

3. Set their speeds to drive:
* `left_motor.motor_control(50);` → Left motor at 50% forward
* `right_motor.motor_control(50);` → Right motor at 50% forward

4. To turn
* Reverse one motor, reduce one speed, or stop one motor depending on turn type.

### Final summary
💡 **_Think of the Motor class as the robot’s accelerator and gear controller combined into a neat software interface – it lets you set how fast and in which direction your wheels spin with a single function call._**
