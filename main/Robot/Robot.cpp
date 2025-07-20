#include "Robot/Robot.h"
#include "IRSensor.h"
#include "esp_timer.h"
#include <cstdint>
#include <ctime>
#include <sys/types.h>

void Robot::init() {
  udpServer.begin(joystick_port);
}

void Robot::update() {
  handleJoystickInput();
  if (_curr_mode == MANUAL) {
    // ...
  }
  else if (_curr_mode == AUTO) { 
    autonomousNav();
  }
}

void Robot::handleJoystickInput() {
  char rx_buffer[MAX_BUF];
  int rcv = udpServer.receive(rx_buffer, sizeof(rx_buffer));
  if (rcv > 0) {
    // ESP_LOGI("Robot", "Received packet: %s", rx_buffer);
    int8_t axes[4];
    int8_t btns[4];
    // NOTE: Parsing using this format => "AXIS:ax1:ax2:ax3:ax4;BTN:btn1:btn2:btn3:btn4"
    char axes_part[64];
    char btns_part[64];

    // NOTE: Parsing explain:
    //          - %63[^;] reads up to 63 characters until it finds a ; — this gets the first part (before ;).
    //          - %63s reads the rest after ; as a string (until whitespace or end).
    int m_type = sscanf(rx_buffer, "%63[^;];%63s", axes_part, btns_part);

    if (m_type == 2) {
      // ESP_LOGI("Robot", "AXIS part: %s", axes_part);
      // ESP_LOGI("Robot", "BTN part: %s", btns_part);
    } else {
      ESP_LOGE("Robot", "Failed to parse input string");
    }

    // Handle buttons first
    int m_btn_vals = sscanf(btns_part, "BTN:%hhd:%hhd:%hhd:%hhd", &btns[0], &btns[1], &btns[2], &btns[3]);
    if (m_btn_vals == 4) {
      // ESP_LOGI("Robot", "Buttons parsed: %d %d %d %d", btns[0], btns[1], btns[2], btns[3]);
      // Handle Buttons
      if (btns[0]) {
        switchMode(MANUAL);
        stopRobot(); // Stop when switching to manual
      }
      else if (btns[1]) {
        switchMode(AUTO);
        robot_state = WAIT; // Reset state when switching to auto
      }
      else if (btns[2]) led.set_on();
      else if (btns[3]) led.set_off();
    } else {
      ESP_LOGE("Robot", "Failed to parse buttons");
    }

    // Only process joystick if in MANUAL mode
    if (_curr_mode == MANUAL) {
      int m_axes_vals = sscanf(axes_part, "AXIS:%hhd:%hhd:%hhd:%hhd", &axes[0], &axes[1], &axes[2], &axes[3]);
      if (m_axes_vals == 4) {
        joystickTeleopControl(axes[1], axes[0]);
      } else {
        ESP_LOGE("Robot", "Failed to parse axes");
      }
    }
  }
}

void Robot::joystickTeleopControl(float axis_left, float axis_right) {
  float norm_left = (axis_left / 127.0f) * 100.0f;
  float norm_right = (axis_right / 127.0f) * 100.0f;
  
  const float deadzone = 10.0f;
  if (abs(axis_left) < deadzone) axis_left = 0;
  if (abs(axis_right) < deadzone) axis_right = 0;

  float left_speed = norm_left + norm_right;
  float right_speed = norm_left - norm_right; 

  left_speed = std::max(-100.0f, std::min(100.0f, left_speed));
  right_speed = std::max(-100.0f, std::min(100.0f, right_speed));

  leftMotor.motor_control(left_speed);
  rightMotor.motor_control(right_speed);

  // ESP_LOGI("Robot", "Motor control: L=%.1f%%, R=%.1f%%", left_speed, right_speed);
}

void Robot::switchMode(ControlMode newMode) {
  if (newMode != _curr_mode) {
    ControlMode prevMode = _curr_mode;
    _curr_mode = newMode;
    // ESP_LOGI("Robot", "Mode change %d to %d", prevMode, _curr_mode);
  }
}

void Robot::autonomousNav() {
    Obstacles obs = checkObstacles();
    uint64_t current_time = esp_timer_get_time() / 1000;

    switch(robot_state) {
        case WAIT:
      timestamp = current_time;
      initMotors();
      robot_state = WAIT_IN_PROGRESS;
      break;

    case WAIT_IN_PROGRESS:
      if (current_time - timestamp > 1000) {
        robot_state = FORWARD;
      }
      break;

    case FORWARD:
      moveForward();
      timestamp = current_time;
      robot_state = FORWARD_IN_PROGESS;
      break;

    case FORWARD_IN_PROGESS:
      if (obs != NO_OBSTACLE) {
        setNextRobotStateInAutoMode(obs);
      } else {
        // Keep moving forward by maintaining motor speeds
        moveForward();
      }
      break;

    case TURN_LEFT:
      turnLeft();
      timestamp = current_time;
      robot_state = TURN_LEFT_IN_PROGRESS;
      break;

    case TURN_LEFT_IN_PROGRESS:
      if (current_time - timestamp > TURN_DURATION_MS) {
        robot_state = FORWARD;
      }
      break;

    case TURN_RIGHT:
      turnRight();
      timestamp = current_time;
      robot_state = TURN_RIGHT_IN_PROGRESS;
      break;

    case TURN_RIGHT_IN_PROGRESS:
      if (current_time - timestamp > TURN_DURATION_MS) {
        robot_state = FORWARD;
      }
      break;

    case TURN_LEFT_IN_PLACE:
      turnLeftInPlace();
      timestamp = current_time;
      robot_state = TURN_LEFT_IN_PLACE_IN_PROGRESS;
      break;

    case TURN_LEFT_IN_PLACE_IN_PROGRESS:
      if (current_time - timestamp > TURN_DURATION_MS) {
        robot_state = FORWARD;
      }
      break;

    case TURN_RIGHT_IN_PLACE:
      turnRightInPlace();
      timestamp = current_time;
      robot_state = TURN_RIGHT_IN_PLACE_IN_PROGRESS;  // FIXED: Correct next state
      break;

    case TURN_RIGHT_IN_PLACE_IN_PROGRESS:
      if (current_time - timestamp > TURN_DURATION_MS) {
        robot_state = FORWARD;
      }
      break;

    default:
      robot_state = WAIT;
      stopRobot();
      break;
  }

  ESP_LOGI("Robot", "Auto State: %d, Obstacles: %d", robot_state, obs);
}

void Robot::setNextRobotStateInAutoMode(Obstacles obs) {
    if (robot_state == FORWARD_IN_PROGESS || robot_state == FORWARD) {
        switch(obs) {
            case NO_OBSTACLE:
                // Don't change state if already moving forward
                break;
            case OBSTACLE_RIGHT:
                robot_state = TURN_LEFT;
                break;
            case OBSTACLE_LEFT:
                robot_state = TURN_RIGHT;
                break;
            case OBSTACLE_FRONT:
                robot_state = TURN_LEFT_IN_PLACE;
                break;
        }
    }
}

Obstacles Robot::checkObstacles() {
  bool left_detect = ir_sensor[0].is_object_detected() || ir_sensor[1].is_object_detected();
  bool right_detect = ir_sensor[3].is_object_detected() || ir_sensor[4].is_object_detected();
  bool front_detect = ir_sensor[2].is_object_detected();

  if (front_detect)
    return OBSTACLE_FRONT;
  else if (left_detect)
    return OBSTACLE_LEFT;
  else if (right_detect)
    return OBSTACLE_RIGHT;
  else
    return NO_OBSTACLE;
}

void Robot::initMotors() {
  rightMotor.init();
  leftMotor.init();
}

void Robot::moveForward() {
  rightMotor.motor_control(85);
  leftMotor.motor_control(85);
}

void Robot::moveBackward() {
  rightMotor.motor_control(-85);
  leftMotor.motor_control(-85);
}

void Robot::turnLeftInPlace() {
  rightMotor.motor_control(-85);
  leftMotor.motor_control(85);
}

void Robot::turnRightInPlace() {
  rightMotor.motor_control(85);
  leftMotor.motor_control(-85);
}

void Robot::turnLeft() {
  rightMotor.motor_control(-85);
  leftMotor.motor_control(85);
}

void Robot::turnRight() {
  rightMotor.motor_control(85);
  leftMotor.motor_control(-85);
}

void Robot::stopRobot() {
  rightMotor.motor_control(0);
  leftMotor.motor_control(0);
}
