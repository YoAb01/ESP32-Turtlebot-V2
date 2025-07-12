#include <stdio.h>
#include "LED/LED.h"
#include "Timer/Timer.h"
#include "Motor/Motor.h"

#define BUILTIN_PIN   GPIO_NUM_2
#define LED_R_PIN     GPIO_NUM_4
#define LED_G_PIN     GPIO_NUM_5

#define MOTOR_A_IN1_PIN GPIO_NUM_16
#define MOTOR_A_IN2_PIN GPIO_NUM_17
#define MOTOR_A_PWM_PIN GPIO_NUM_18

#define MOTOR_B_IN1_PIN GPIO_NUM_42
#define MOTOR_B_IN2_PIN GPIO_NUM_41
#define MOTOR_B_PWM_PIN GPIO_NUM_40

LED builtin_led(BUILTIN_PIN);
LED red_led(LED_R_PIN);
LED green_led(LED_G_PIN);
Motor motorA(MOTOR_A_IN1_PIN, MOTOR_A_IN2_PIN, MOTOR_A_PWM_PIN);
Motor motorB(MOTOR_B_IN1_PIN, MOTOR_B_IN2_PIN, MOTOR_B_PWM_PIN);


void test_robot_motion(void *pvParameter) {
  while (1) {
      ESP_LOGI("MOTOR", "Moving forward...");
      motorA.motor_control(100);
      motorB.motor_control(100);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      ESP_LOGI("MOTOR", "Moving backward...");
      motorA.motor_control(-100);
      motorB.motor_control(-100);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      ESP_LOGI("MOTOR", "Turning left...");
      motorA.motor_control(100);
      motorB.motor_control(-100);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      ESP_LOGI("MOTOR", "Turning right...");
      motorA.motor_control(-100);
      motorB.motor_control(100);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      motorA.stop();
      motorB.stop();
      vTaskDelay(5000 / portTICK_PERIOD_MS);
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

  // Motor control
  motorA.init();
  motorB.init();

  xTaskCreate(test_robot_motion, "robot_movement_task", 2048, NULL, 5, NULL);



}
