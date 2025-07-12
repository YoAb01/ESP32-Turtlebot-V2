
#ifndef MOTOR_H
#define MOTOR_H

#include <cstdint>
#include <stdio.h>
#include "driver/mcpwm.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "soc/gpio_num.h"


class Motor {
public:
  Motor(gpio_num_t in1_pin, gpio_num_t in2_pin, gpio_num_t pwm_pin)
    : _in1_pin(in1_pin), _in2_pin(in2_pin), _pwm_pin(pwm_pin) {}

  void init(uint32_t freq_hz = 1000);
  void motor_control(float duty_percent);
  void stop();

private:
  gpio_num_t _in1_pin;    // Directional control input 1 (IN_1)
  gpio_num_t _in2_pin;    // Directional control input 2 (IN_2)
  gpio_num_t _pwm_pin;    // Speed Control (via PWM signal)
};

#endif // MOTOR_H
