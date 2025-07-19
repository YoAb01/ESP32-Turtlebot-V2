#include <stdio.h>
#include "LED/LED.h"
#include "Timer/Timer.h"
#include "Motor/Motor.h"
#include "WiFi/WiFiManager.h"
#include "freertos/idf_additions.h"
#include "wifi_confidentials.h"
#include "Robot/Robot.h"

#define BUILTIN_PIN   GPIO_NUM_2
#define LED_R_PIN     GPIO_NUM_4
#define LED_G_PIN     GPIO_NUM_5

#define I2C_MASTER_SCK_IO           GPIO_NUM_21
#define I2C_MASTER_SDA_IO           GPIO_NUM_47

#define IR_CENTER_PIN   GPIO_NUM_20

LED builtin_led(BUILTIN_PIN);
LED red_led(LED_R_PIN);
LED green_led(LED_G_PIN);
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
    robot->handleJoystickInput();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

extern "C" void app_main(void)
{
  builtin_led.init();
  red_led.init();
  green_led.init();

  // Blink with GPTimer
  green_led.blink_timer(1000000);  // 1 second
  red_led.blink_timer(3000000);    // 3 seconds

  robot.init();

  // WiFi Local Network Connection
  init_wifi_sta();

  // Robot task
  xTaskCreate(joystick_robot_teleop_task, "joystick_task", 4096, &robot, 6, NULL);

}
