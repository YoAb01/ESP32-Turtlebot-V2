#include "Robot/Robot.h"

void Robot::init() {
  udpServer.begin(joystick_port);
}

void Robot::update() {
  handleJoystickInput();
}

void Robot::handleJoystickInput() {
  char rx_buffer[MAX_BUF];
  int rcv = udpServer.receive(rx_buffer, sizeof(rx_buffer));
  if (rcv > 0) {
    ESP_LOGI("Robot", "Received packet: %s", rx_buffer);
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
      ESP_LOGI("Robot", "AXIS part: %s", axes_part);
      ESP_LOGI("Robot", "BTN part: %s", btns_part);
    } else {
      ESP_LOGE("Robot", "Failed to parse input string");
    }

    int m_axes_vals = sscanf(axes_part, "AXIS:%hhd:%hhd:%hhd:%hhd", &axes[0], &axes[1], &axes[2], &axes[3]);
    if (m_axes_vals == 4) {
      ESP_LOGI("Robot", "Axes parsed: %d %d %d %d", axes[0], axes[1], axes[2], axes[3]);
      // Handle joystick teleop
      joystickTeleopControl(axes[0], axes[1]);
    } else {
      ESP_LOGE("Robot", "Failed to parse axes");
    }

    int m_btn_vals = sscanf(btns_part, "BTN:%hhd:%hhd:%hhd:%hhd", &btns[0], &btns[1], &btns[2], &btns[3]);
    if (m_btn_vals == 4) {
      ESP_LOGI("Robot", "Buttons parsed: %d %d %d %d", btns[0], btns[1], btns[2], btns[3]);
      // Handle Buttons
      if (btns[1]) led.set_on();
      else led.set_off();
    } else {
      ESP_LOGE("Robot", "Failed to parse buttons");
    }
  }
}

void Robot::joystickTeleopControl(float axis_left, float axis_right) {
  float norm_left = (axis_left / 32767.0f) * 100.0f;
  float norm_right = (axis_right / 32767.0f) * 100.0f;
  
  const float deadzone = 10.0f;
  if (abs(axis_left) < deadzone) axis_left = 0;
  if (abs(axis_right) < deadzone) axis_right = 0;

  float left_speed = norm_left + norm_right;
  float right_speed = norm_left - norm_right; 

  left_speed = std::max(-100.0f, std::min(100.0f, left_speed));
  right_speed = std::max(-100.0f, std::min(200.0f, right_speed));

  leftMotor.motor_control(left_speed);
  rightMotor.motor_control(right_speed);

  ESP_LOGI("Robot", "Motor control: L=%.1f%%, R=%.1f%%", left_speed, right_speed);
}
