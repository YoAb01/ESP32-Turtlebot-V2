#include "Motor/Motor.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include "driver/mcpwm_types_legacy.h"
#include "esp_err.h"


void Motor::init(uint32_t freq_hz)
{
  // Configure pin direction
  gpio_config_t io_conf = {};
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = (1ULL << _in1_pin) | (1ULL << _in2_pin);
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  ESP_ERROR_CHECK(gpio_config(&io_conf));

  // Configure MCPWM GPIO Pin
  ESP_ERROR_CHECK(mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, _pwm_pin));
  
  // Configure PWM
  mcpwm_config_t pwm_config = {};
  pwm_config.frequency = freq_hz;
  pwm_config.cmpr_a = 0;
  pwm_config.cmpr_b = 0;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;

  // Init MCPWM
  ESP_ERROR_CHECK(mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config));
}

void Motor::motor_control(float duty_percent) {
  // Limit/Clamp rannge from -100 to 100 %
  if (duty_percent > 100.0f) duty_percent = 100.0f;
  if (duty_percent < -100.0f) duty_percent = -100.0f;

  // Forward
  if (duty_percent > 0) {
    gpio_set_level(_in1_pin, 1);
    gpio_set_level(_in2_pin, 0);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_percent);
  } 
  // Backward
  else if (duty_percent < 0) {
    gpio_set_level(_in1_pin, 0);
    gpio_set_level(_in2_pin, 1);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, -duty_percent);
  } 
  // Stop
  else {
    gpio_set_level(_in1_pin, 0);
    gpio_set_level(_in2_pin, 0);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 0);
  }
}

void Motor::stop() {
  motor_control(0.0);
}
