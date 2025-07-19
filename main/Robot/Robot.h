#ifndef ROBOT_H
#define ROBOT_H

#include <cstdint>
#include <stdio.h>
#include "driver/mcpwm.h"
#include "driver/gpio.h"
#include "esp_err.h"

#include "LED/LED.h"
#include "Motor/Motor.h"
#include "WiFiManager.h"
#include "UDPServer.h"

#define MAX_BUF 128

#define MOTOR_A_IN1_PIN GPIO_NUM_37
#define MOTOR_A_IN2_PIN GPIO_NUM_38
#define MOTOR_A_PWM_PIN GPIO_NUM_39

#define MOTOR_B_IN1_PIN GPIO_NUM_42
#define MOTOR_B_IN2_PIN GPIO_NUM_41
#define MOTOR_B_PWM_PIN GPIO_NUM_40

enum ControlMode {
  MANUAL,
  AUTO
};

class Robot {
public:
  Robot() 
    : led(GPIO_NUM_2),  // Initialize with builtin LED pin
      rightMotor(MOTOR_A_IN1_PIN, MOTOR_A_IN2_PIN, MOTOR_A_PWM_PIN),
      leftMotor(MOTOR_B_IN1_PIN, MOTOR_B_IN2_PIN, MOTOR_B_PWM_PIN)
  {
      
  }

  void init();
  void update();

  void handleJoystickInput();
  void joystickTeleopControl(float axis_left, float axis_right);

private:
  LED led;
  Motor rightMotor;
  Motor leftMotor;
  WiFiManager wifi;
  UDPServer udpServer;

  const uint16_t joystick_port = 4201;
};


#endif  // ROBOT_H
