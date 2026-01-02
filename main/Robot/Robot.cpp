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
    int8_t axes[4];
    int8_t btns[4];
    char axes_part[64];
    char btns_part[64];

    int m_type = sscanf(rx_buffer, "%63[^;];%63s", axes_part, btns_part);

    if (m_type == 2) {
      // Handle buttons first
      int m_btn_vals = sscanf(btns_part, "BTN:%hhd:%hhd:%hhd:%hhd", &btns[0], &btns[1], &btns[2], &btns[3]);
      if (m_btn_vals == 4) {
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
    } else {
      ESP_LOGE("Robot", "Failed to parse input string");
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

}

void Robot::switchMode(ControlMode newMode) {
  if (newMode != _curr_mode) {
    ControlMode prevMode = _curr_mode;
    _curr_mode = newMode;
    ESP_LOGI("Robot", "Mode change %d to %d", prevMode, _curr_mode);
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
      robot_state = TURN_RIGHT_IN_PLACE_IN_PROGRESS;
      break;

    case TURN_RIGHT_IN_PLACE_IN_PROGRESS:
      if (current_time - timestamp > TURN_DURATION_MS) {
        robot_state = FORWARD;
      }
      break;
    case BACKWARD:
      moveBackward();
      timestamp = current_time;
      robot_state = BACKWARD_IN_PROGRESS;
      break;

    case BACKWARD_IN_PROGRESS:
      if (current_time - timestamp > BACKWARD_DURATION_MS) {
        robot_state = TURN_LEFT_IN_PLACE;
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
            case OBSTACLE_FRONT:
                robot_state = shouldReverse() ? BACKWARD : TURN_LEFT_IN_PLACE;
                break;
            case OBSTACLE_LEFT:
                robot_state = TURN_RIGHT;
                break;
            case OBSTACLE_RIGHT:
                robot_state = TURN_LEFT;
                break;
        }
    }
}

Obstacles Robot::checkObstacles() {
  uint8_t sensor_mask = getCurrentSensorMask();
  switch (sensor_mask) {
    // No obstacles
    case 0b00000:
      return NO_OBSTACLE;
    // Front obstacles + variations
    case 0b00100:  // Center only
    case 0b01100:  // Center + Left Inner
    case 0b00110:  // Center + Right Inner
    case 0b01110:  // Center + Left/Right Inner
    case 0b11100:  // Center + Left Inner/Outer
    case 0b00111:  // Center + Right Inner/Outer
    case 0b11111:  // All sensors (big obstacle)
      return OBSTACLE_FRONT;

    // Left obstacles only
    case 0b10000:  // Left outer only
    case 0b01000:  // Left inner only
    case 0b11000:  // Both left sensors
    case 0b10001:  // Left outer + right outer (prefer left turn)
    case 0b01010:  // Left inner + right inner (prefer left turn)
      return OBSTACLE_LEFT;

    // Right obstacles only
    case 0b00001:  // Right outer only
    case 0b00010:  // Right inner only
    case 0b00011:  // Both right sensors
      return OBSTACLE_RIGHT;

    default:
      return resolveComplexPattern(sensor_mask);

  }
}

Obstacles Robot::resolveComplexPattern(uint8_t sensor_mask) {
  // Center is the highest priority
  if (sensor_mask & (1 << SENSOR_CENTER)) {
    return OBSTACLE_FRONT;
  }
  uint8_t left_mask = sensor_mask & 0b11000;   // Isolate left sensors
  uint8_t right_mask = sensor_mask & 0b00011;  // Isolate right sensors

  int left_count = __builtin_popcount(left_mask);   // Count set bits
  int right_count = __builtin_popcount(right_mask);

  if (left_count > right_count) {
    return OBSTACLE_LEFT;
  } else if (right_count > left_count) {
    return OBSTACLE_RIGHT;
  } else {
    // Equal counts - use deterministic tie-breaker
    return (sensor_mask & (1 << SENSOR_LEFT_OUTER)) ? OBSTACLE_LEFT : OBSTACLE_RIGHT;
  }
}

uint8_t Robot::getCurrentSensorMask() {
  uint8_t mask = 0;
  for (int i = 0; i < 5; i++) {
    if (ir_sensor[i].is_object_detected()) {
      mask |= (1 << i);
    }
  }
  ESP_LOGE("Robot", "Mask: %u", mask);
  return mask;
}

bool Robot::shouldReverse() {
  uint8_t sensor_mask = getCurrentSensorMask();
  int active_sensors = __builtin_popcount(sensor_mask);
  return (active_sensors >= 4);  // Reverse if 4 or 5 sensors active
}

void Robot::initMotors() {
  rightMotor.init();
  leftMotor.init();
}

void Robot::moveForward() {
  rightMotor.motor_control(80);
  leftMotor.motor_control(80);
}

void Robot::moveBackward() {
  rightMotor.motor_control(-80);
  leftMotor.motor_control(-80);
}

void Robot::turnLeftInPlace() {
  rightMotor.motor_control(80);
  leftMotor.motor_control(-80);
}

void Robot::turnRightInPlace() {
  rightMotor.motor_control(-80);
  leftMotor.motor_control(80);
}

void Robot::turnLeft() {
  rightMotor.motor_control(80);
  leftMotor.motor_control(-75);
}

void Robot::turnRight() {
  rightMotor.motor_control(-75);
  leftMotor.motor_control(80);
}

void Robot::stopRobot() {
  rightMotor.motor_control(0);
  leftMotor.motor_control(0);
}
