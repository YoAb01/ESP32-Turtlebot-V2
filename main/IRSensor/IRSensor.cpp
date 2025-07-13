#include "IRSensor/IRSensor.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "soc/gpio_num.h"


void IRSensor::init() {
  gpio_reset_pin(_pin_num);
  gpio_set_direction(_pin_num, GPIO_MODE_INPUT);
}

bool IRSensor::is_object_detected() {
  return !gpio_get_level(_pin_num);
}
