#ifndef IMU_H
#define IMU_H

#include <stdio.h>
#include <cstdint>
#include "driver/gptimer_types.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "soc/gpio_num.h"
#include "esp_log.h"
#include "hal/i2c_types.h"
#include "driver/i2c.h"
#include <math.h>


#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          400000
#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0
#define I2C_TIMEOUT_MS              1000

#define MPU6050_ADDR                0x68
#define MPU6050_PWM_MGMT_1_REG      0x6B
#define MPU6050_REG_DATA            0x3B


class IMU {
public:
  IMU(gpio_num_t sda_pin, gpio_num_t sck_pin) : _sda_pin(sda_pin), _sck_pin(sck_pin) {}

  void init_imu();
  void read_data(double* accelX, double* accelY, double* accelZ, double* gyroX, double* gyroY, double* gyroZ, double* tempC);

private:
  gpio_num_t _sda_pin;
  gpio_num_t _sck_pin;
};

#endif  // IMU_H

