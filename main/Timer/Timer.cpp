#include "Timer/Timer.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_err.h"
#include "hal/gpio_types.h"
#include "hal/timer_types.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"
#include <cstdint>


void Timer::init() {
  gptimer_config_t timer_config = {};
  timer_config.clk_src = _clk_src;
  timer_config.direction = _direction;
  timer_config.resolution_hz = 1000000;
  timer_config.intr_priority = 0;
  ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &_timer_handle));
}

void Timer::start() {
  ESP_ERROR_CHECK(gptimer_enable(_timer_handle));
  ESP_ERROR_CHECK(gptimer_start(_timer_handle));
}

void Timer::stop() {
  ESP_ERROR_CHECK(gptimer_stop(_timer_handle));
  ESP_ERROR_CHECK(gptimer_disable(_timer_handle));
  ESP_ERROR_CHECK(gptimer_del_timer(_timer_handle));
}

void Timer::attach_callback(gptimer_alarm_cb_t callback, void* user_data) {
  gptimer_event_callbacks_t cbs;
  cbs.on_alarm = callback;
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(_timer_handle, &cbs, user_data));
}

void Timer::change_period(uint64_t period_us) {
  _period_us = period_us;
  gptimer_alarm_config_t alarm_config = {};
  alarm_config.reload_count = 0;
  alarm_config.alarm_count = period_us;
  alarm_config.flags.auto_reload_on_alarm = true;
  ESP_ERROR_CHECK(gptimer_set_alarm_action(_timer_handle, &alarm_config));
}

void Timer::configure_pin(gpio_num_t pin_num, gpio_mode_t pin_mode) {
  gpio_config_t io_config = {};
  io_config.pin_bit_mask = (1ULL << pin_num);
  io_config.mode = pin_mode;
  io_config.pull_up_en = GPIO_PULLUP_DISABLE;
  io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_config.intr_type = GPIO_INTR_DISABLE;
  ESP_ERROR_CHECK(gpio_config(&io_config));
}
