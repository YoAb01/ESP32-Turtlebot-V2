#include <stdio.h>
#include "LED/LED.h"

LED led_red(GPIO_NUM_2);

extern "C" void app_main(void)
{
  // LED blink task
  led_red.init();
  xTaskCreate(
      LED::_blink_task_w,  // Task function ptr
      "LED_Blink_task",        // Task name
      2048,              // Stack size
      &led_red,          // LED instance
      1,                   // Priority
      NULL              // Task Handle (NULL here)
    );
}
