#include <stdio.h>
#include "LED/LED.h"
#include "WiFi/WiFiManager.h"
#include "freertos/idf_additions.h"
#include "soc/gpio_num.h"
#include "wifi_confidentials.h"
#include "Robot/Robot.h"
#include "IMU/IMU.h"
#include "IMU/IMUSender.h"

#define I2C_MASTER_SCK_IO           GPIO_NUM_9
#define I2C_MASTER_SDA_IO           GPIO_NUM_8

WiFiManager wifi_ap;
IMU mpu6050(I2C_MASTER_SDA_IO, I2C_MASTER_SCK_IO);
IMUSender imu_sender("192.168.1.167", 2525);


void init_wifi_sta() {
  const char* ssid = WIFI_SSID_CONN;
  const char* pwd = WIFI_PASS_CONN;

  if (wifi_ap.init_STA(ssid, pwd)) {
    ESP_LOGI("WIFI", "Connected to WiFi!");
  } else {
    ESP_LOGE("WIFI", "Failed to connect");
  }
}

void test_imu_data(void *pvParameter) {
  while (1) {
    double ax, ay, az, gx, gy, gz, tempC;
    mpu6050.read_data(&ax, &ay, &az, &gx, &gy, &gz, &tempC);
    imu_sender.send_imu_data(ax, ay, az, gx, gy, gz, tempC);

    static int count = 0;
    if (++count % 20 == 0) {
      printf("Accel (g): X=%.2f Y=%.2f Z=%.2f | Gyro (°/s): X=%.2f Y=%.2f Z=%.2f | Temp (°C): T=%.2f\n",
               ax, ay, az, gx, gy, gz, tempC);
    }
    vTaskDelay(pdMS_TO_TICKS(10));       // 100Hz update rate
  }
}

extern "C" void app_main(void)
{
  // Init Wifi
  init_wifi_sta();

  // Init IMU
  mpu6050.init_imu();

  if (!imu_sender.init()) {
    ESP_LOGE("MAIN", "Failed to init IMU sender!");
  }

  xTaskCreate(test_imu_data, "imu_data_task", 8192, NULL, 4, NULL);

  ESP_LOGI("MAIN", "IMU data streaming started!");
}
