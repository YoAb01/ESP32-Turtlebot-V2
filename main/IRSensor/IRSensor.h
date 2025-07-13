#ifndef IRSENSOR_H
#define IRSENSOR_H

#include <stdio.h>
#include <cstdint>
#include "driver/gpio.h"
#include "esp_log.h"


class IRSensor {
public:
  IRSensor(gpio_num_t pin_num) : _pin_num(pin_num) {}

  void init();
  bool is_object_detected();

private:
  gpio_num_t _pin_num;

};

#endif  // IRSENSOR_H
