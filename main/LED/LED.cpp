#include "LED/LED.h"
#include "driver/gpio.h"
#include "hal/timer_types.h"
#include "soc/clk_tree_defs.h"
#include <cstdint>


void LED::init() {
  // Configure the pin number by reseting and setting to OUTPUT mode
  gpio_reset_pin(_pin_num);
  gpio_set_direction(_pin_num, GPIO_MODE_OUTPUT);
  gpio_set_level(_pin_num, 0);   // LED starts off
}

int LED::get_status() {
  // Read the GPIO level of that PIN 
  _status = gpio_get_level(_pin_num);
  if (_status == 0) {
    ESP_LOGI(LED_TAG, "LED at pin %i is OFF", _pin_num);
    return 0;
  }
  else if (_status == 1) {
    ESP_LOGI(LED_TAG, "LED at pin %i is ON", _pin_num);
    return 1;
  }
  else
    return -1;
}

void LED::set_on() {
  ESP_LOGI(LED_TAG, "Setting the LED at pin %i to HIGH", _pin_num);
  gpio_set_level(_pin_num, 1);
}

void LED::set_off() {
  ESP_LOGI(LED_TAG, "Setting the LED at pin %i to LOW", _pin_num);
  gpio_set_level(_pin_num, 0);
}

void LED::blink() {
  ESP_LOGI(LED_TAG, "Start blinking the LED at %i interval", _blink_interval);
  set_on();
  vTaskDelay(_blink_interval / portTICK_PERIOD_MS);
  set_off();
  vTaskDelay(_blink_interval / portTICK_PERIOD_MS);
}

void LED::blink_task() {
  while (true) {
    blink();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void LED::blink_timer(uint64_t period_us) {
  _blink_timer = Timer(GPTIMER_COUNT_UP, GPTIMER_CLK_SRC_DEFAULT, period_us);
  _blink_timer.attach_callback(_timer_callback, this);
  _blink_timer.change_period(period_us);
  _blink_timer.start();
}

bool LED::_timer_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data) {
  LED *led_instance = static_cast<LED*>(user_data);
  int status = gpio_get_level(led_instance->_pin_num);
  gpio_set_level(led_instance->_pin_num, !status);
  ESP_LOGI("LED_CALLBACK", "LED toggled");
  return true;
}


void LED::fade() {
  //TODO: Implement fading LED using PWM principles
}
