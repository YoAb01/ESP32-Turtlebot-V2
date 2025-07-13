#include "IMU/IMU.h"
#include "hal/i2c_types.h"


void IMU::init_imu() {
  i2c_config_t i2c_config {};
  i2c_config.mode = I2C_MODE_MASTER;
  i2c_config.scl_io_num = _sck_pin;
  i2c_config.sda_io_num = _sda_pin;
  i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
  i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
  i2c_config.master.clk_speed = I2C_MASTER_FREQ_HZ;

  ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &i2c_config));
  ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER,
                                    I2C_MASTER_RX_BUF_DISABLE,
                                    I2C_MASTER_TX_BUF_DISABLE, 0));
  uint8_t wake_cmd[2] = {MPU6050_PWM_MGMT_1_REG, 0x00};
  ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_MASTER_NUM,
                                            MPU6050_ADDR,
                                            wake_cmd,
                                            sizeof(wake_cmd),
                                            pdMS_TO_TICKS(I2C_TIMEOUT_MS)));

  ESP_LOGI("IMU", "MPU6050 initialized on SDA:%d, SCL:%d",
            _sda_pin,
            _sck_pin);
}

void IMU::read_data(double* accelX, double* accelY, double* accelZ, double* gyroX, double* gyroY, double* gyroZ, double* tempC) {
  // Placeholder for raw data: Accel(6) - Temp(2) - Gyro(6)
  uint8_t raw_data[14];
  uint8_t reg_data = MPU6050_REG_DATA;
  i2c_master_write_read_device(I2C_MASTER_NUM,
                                 MPU6050_ADDR,
                                 &reg_data, 1,
                                 raw_data, 14,
                                 I2C_TIMEOUT_MS / portTICK_PERIOD_MS);

  // Combine bytes into signed 16-bit integers
  int16_t ax_raw = (raw_data[0] << 8) | raw_data[1];
  int16_t ay_raw = (raw_data[2] << 8) | raw_data[3];
  int16_t az_raw = (raw_data[4] << 8) | raw_data[5];

  int16_t temp = (raw_data[6] << 8) | raw_data[7];

  int16_t gx_raw = (raw_data[8] << 8) | raw_data[9];
  int16_t gy_raw = (raw_data[10] << 8) | raw_data[11];
  int16_t gz_raw = (raw_data[12] << 8) | raw_data[13];

  // Convert into physical units
  *accelX = (ax_raw / 16384.0) * 9.80665;
  *accelY = (ay_raw / 16384.0) * 9.80665;
  *accelZ = (az_raw / 16384.0) * 9.80665;

  *gyroX = (gx_raw / 131.0) * (M_PI / 180.0);
  *gyroY = (gy_raw / 131.0) * (M_PI / 180.0);
  *gyroZ = (gz_raw / 131.0) * (M_PI / 180.0);

  *tempC = (temp / 340.0) + 36.53;
}
