#ifndef LED_H
#define LED_H

#include <cstdint>
#include <stdio.h>
#include "driver/gptimer_types.h"
#include "driver/timer.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/gpio_num.h"
#include "esp_log.h"

#include "Timer/Timer.h"

#define LED_TAG "LED"

class LED {
public:
  LED(gpio_num_t pin, int blink_interval = 500, int fade_amount = 5)
    : _pin_num(pin), _blink_interval(blink_interval), _fade_amount(fade_amount)
  {
    ESP_LOGI(LED_TAG, "LED instance added at pin %i", pin);
  }

  void init();
  int get_status();
  void set_blink_interval(int interval);
  void set_on();
  void set_off();
  void blink();
  void blink_timer(uint64_t period_us);
  void fade();

  // NOTE: This is been defined as static because FreeRTOS requires a C-style function pointer and not a class method that implicitly has "this" as first parameter refering to the object
  static void _blink_task_w(void *pvParameters){
    LED *led_instance = static_cast<LED*>(pvParameters);
    led_instance->blink_task();
  }

  void blink_task();


private:
  gpio_num_t _pin_num;
  int _blink_interval = 500;
  int _fade_amount = 5;
  int _status = 0;

  Timer _blink_timer;
  static bool _timer_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data);
};

#endif
