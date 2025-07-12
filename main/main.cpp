#include <stdio.h>
#include "LED/LED.h"
#include "Timer/Timer.h"

#define BUILTIN_PIN GPIO_NUM_2
#define LED_R_PIN   GPIO_NUM_4
#define LED_B_PIN   GPIO_NUM_5


LED builtin_led(BUILTIN_PIN);
LED red_led(LED_R_PIN);
LED blue_led(LED_B_PIN);


extern "C" void app_main(void)
{

  builtin_led.init();
  red_led.init();

  // LED blink task
  // builtin_led.set_blink_interval(1000);
  // red_led.set_blink_interval(5000);

  // xTaskCreate(LED::_blink_task_w, "red_led_blink_task", 2048, &red_led, 1, NULL);
  // xTaskCreate(LED::_blink_task_w, "builtin_led_blink_task", 2048, &builtin_led, 1, NULL);

  builtin_led.blink_timer(200000);  // 0.2 seconds
  red_led.blink_timer(3000000);     // 3 second
}
