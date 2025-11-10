#include <stdio.h>
#include "LED/LED.h"
#include "WiFi/WiFiManager.h"
#include "freertos/idf_additions.h"
#include "soc/gpio_num.h"
#include "wifi_confidentials.h"
#include "Robot/Robot.h"
#include "IMU/IMU.h"

#define I2C_MASTER_SCK_IO           GPIO_NUM_9
#define I2C_MASTER_SDA_IO           GPIO_NUM_8

IMU mpu6050(I2C_MASTER_SDA_IO, I2C_MASTER_SCK_IO);

void test_imu_data(void *pvParameter) {
  while (1) {
    double ax, ay, az, gx, gy, gz, tempC;
    mpu6050.read_data(&ax, &ay, &az, &gx, &gy, &gz, &tempC);
    printf("Accel (g): X=%.2f Y=%.2f Z=%.2f | Gyro (°/s): X=%.2f Y=%.2f Z=%.2f | Temp (°C): T=%.2f\n",
               ax, ay, az, gx, gy, gz, tempC);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

extern "C" void app_main(void)
{
  mpu6050.init_imu();
  xTaskCreate(test_imu_data, "imu_data_task", 2048, NULL, 4, NULL);
}
