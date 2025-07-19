#include <stdio.h>
#include "LED/LED.h"
#include "WiFi/WiFiManager.h"
#include "freertos/idf_additions.h"
#include "wifi_confidentials.h"
#include "Robot/Robot.h"

WiFiManager wifi_ap;
Robot robot;

void init_wifi_ap() {
  const char* ssid = "ESP32-Robot";
  const char* pwd = "password_esp32_2025";

  if (wifi_ap.init_AP(ssid, pwd)) {
    ESP_LOGI("WIFI", "Access Point started successfully!");
  } else {
    ESP_LOGE("WIFI", "Failed to start Access Point.");
  }
}

void init_wifi_sta() {
  const char* ssid = WIFI_SSID_CONN;
  const char* pwd = WIFI_PASS_CONN;

  if (wifi_ap.init_STA(ssid, pwd)) {
    ESP_LOGI("WIFI", "Connected Successfully to local network");
  } else {
    ESP_LOGE("WIFI", "Failed to connect to local network");
  }
}

void joystick_robot_teleop_task(void *pvParameter) {
  Robot *robot = (Robot *)pvParameter;
  while (1) {
    robot->update();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

extern "C" void app_main(void)
{
  init_wifi_sta();
  robot.init();
  xTaskCreate(joystick_robot_teleop_task, "joystick_task", 4096, &robot, 6, NULL);
}
