#ifndef ROBOT_H
#define ROBOT_H

#include <cstdint>
#include <stdio.h>
#include <vector>
#include "driver/mcpwm.h"
#include "driver/gpio.h"
#include "esp_err.h"

#include "LED/LED.h"
#include "Motor/Motor.h"
#include "WiFiManager.h"
#include "UDPServer.h"
#include "IRSensor/IRSensor.h"

#define MAX_BUF 128
#define TURN_DURATION_MS 400
#define BACKWARD_DURATION_MS 500

#define MOTOR_A_IN1_PIN GPIO_NUM_27
#define MOTOR_A_IN2_PIN GPIO_NUM_14
#define MOTOR_A_PWM_PIN GPIO_NUM_26

#define MOTOR_B_IN1_PIN GPIO_NUM_33
#define MOTOR_B_IN2_PIN GPIO_NUM_32
#define MOTOR_B_PWM_PIN GPIO_NUM_5

#define IR_SENSOR_1 GPIO_NUM_43
#define IR_SENSOR_2 GPIO_NUM_42
#define IR_SENSOR_3 GPIO_NUM_48
#define IR_SENSOR_4 GPIO_NUM_19
#define IR_SENSOR_5 GPIO_NUM_18

#define SENSOR_LEFT_OUTER  0
#define SENSOR_LEFT_INNER  1
#define SENSOR_CENTER      2
#define SENSOR_RIGHT_INNER 3
#define SENSOR_RIGHT_OUTER 4

enum ControlMode {
  MANUAL,
  AUTO
};

enum RobotState {
  WAIT,
  WAIT_IN_PROGRESS,
  FORWARD,
  FORWARD_IN_PROGESS,
  TURN_LEFT,
  TURN_LEFT_IN_PROGRESS,
  TURN_RIGHT,
  TURN_RIGHT_IN_PROGRESS,
  TURN_LEFT_IN_PLACE,
  TURN_LEFT_IN_PLACE_IN_PROGRESS,
  TURN_RIGHT_IN_PLACE,
  TURN_RIGHT_IN_PLACE_IN_PROGRESS,
  STOP,
  STOP_IN_PROGRESS,
  BACKWARD,
  BACKWARD_IN_PROGRESS,
};

enum Obstacles {
  NO_OBSTACLE,
  OBSTACLE_LEFT,
  OBSTACLE_RIGHT,
  OBSTACLE_FRONT
};

class Robot {
public:
  Robot()
    : led(GPIO_NUM_2),
      rightMotor(MOTOR_A_IN1_PIN, MOTOR_A_IN2_PIN, MOTOR_A_PWM_PIN),
      leftMotor(MOTOR_B_IN1_PIN, MOTOR_B_IN2_PIN, MOTOR_B_PWM_PIN),
      udpServer(), 
      _curr_mode(MANUAL)
  {
    initMotors();
    ir_sensor = {
      IRSensor(IR_SENSOR_1),
      IRSensor(IR_SENSOR_2),
      IRSensor(IR_SENSOR_3),
      IRSensor(IR_SENSOR_4),
      IRSensor(IR_SENSOR_5)
    };
  }

  void init();
  void update();

  void handleJoystickInput();
  void joystickTeleopControl(float axis_left, float axis_right);

  void switchMode(ControlMode newMode);

  void autonomousNav();
  Obstacles checkObstacles();
  void setNextRobotStateInAutoMode(Obstacles obs);
  void initMotors();
  void moveForward();
  void moveBackward();
  void turnLeft();
  void turnRight();
  void turnLeftInPlace();
  void turnRightInPlace();
  void stopRobot();

private:
  LED led;
  Motor rightMotor;
  Motor leftMotor;
  WiFiManager wifi;
  UDPServer udpServer;
  std::vector<IRSensor> ir_sensor;
  ControlMode _curr_mode;
  RobotState robot_state = WAIT;
  RobotState next_robot_state;
  uint64_t timestamp = 0;

  const uint16_t joystick_port = 4201;

  Obstacles resolveComplexPattern(uint8_t sensor_mask);
  bool shouldReverse();
  uint8_t getCurrentSensorMask();  
};


#endif  // ROBOT_H
