#ifndef TIMER_H
#define TIMER_H

#include <cstdint>
#include <stdio.h>
#include "driver/gptimer.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "hal/gpio_types.h"
#include "soc/gpio_num.h"


class Timer {
public:
  Timer(gptimer_count_direction_t direction = GPTIMER_COUNT_UP, gptimer_clock_source_t clk_src = GPTIMER_CLK_SRC_DEFAULT, uint32_t period_us = 1000000) 
    : _period_us(period_us), _direction(direction), _clk_src(clk_src) {}
  void start();
  void stop();
  void change_period(uint64_t period_us);
  void attach_callback(gptimer_alarm_cb_t callback, void* user_data);
  void configure_pin(gpio_num_t pin_num, gpio_mode_t mode);

private:
  gptimer_handle_t _timer_handle;
  uint64_t _period_us;
  gptimer_count_direction_t _direction = GPTIMER_COUNT_UP;
  gptimer_clock_source_t _clk_src = GPTIMER_CLK_SRC_DEFAULT;
};

#endif   // TIMER_H
