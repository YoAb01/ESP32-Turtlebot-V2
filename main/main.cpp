#include <stdio.h>
#include "LED/LED.h"
#include "Timer/Timer.h"
#include "Motor/Motor.h"
#include "IMU/IMU.h"
#include "IRSensor/IRSensor.h"

#define BUILTIN_PIN   GPIO_NUM_2
#define LED_R_PIN     GPIO_NUM_4
#define LED_G_PIN     GPIO_NUM_5

#define MOTOR_A_IN1_PIN GPIO_NUM_37
#define MOTOR_A_IN2_PIN GPIO_NUM_38
#define MOTOR_A_PWM_PIN GPIO_NUM_39

#define MOTOR_B_IN1_PIN GPIO_NUM_42
#define MOTOR_B_IN2_PIN GPIO_NUM_41
#define MOTOR_B_PWM_PIN GPIO_NUM_40

#define I2C_MASTER_SCK_IO           GPIO_NUM_21
#define I2C_MASTER_SDA_IO           GPIO_NUM_47

#define IR_CENTER_PIN   GPIO_NUM_20

LED builtin_led(BUILTIN_PIN);
LED red_led(LED_R_PIN);
LED green_led(LED_G_PIN);
Motor motorA(MOTOR_A_IN1_PIN, MOTOR_A_IN2_PIN, MOTOR_A_PWM_PIN);
Motor motorB(MOTOR_B_IN1_PIN, MOTOR_B_IN2_PIN, MOTOR_B_PWM_PIN);
IMU mpu6050(I2C_MASTER_SDA_IO, I2C_MASTER_SCK_IO);
IRSensor ir_center(IR_CENTER_PIN);

void test_robot_motion(void *pvParameter) {
  while (1) {
      ESP_LOGI("MOTOR", "Moving forward...");
      motorA.motor_control(80);
      motorB.motor_control(80);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      ESP_LOGI("MOTOR", "Moving backward...");
      motorA.motor_control(-80);
      motorB.motor_control(-80);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      ESP_LOGI("MOTOR", "Turning left...");
      motorA.motor_control(80);
      motorB.motor_control(-80);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      ESP_LOGI("MOTOR", "Turning right...");
      motorA.motor_control(-80);
      motorB.motor_control(80);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      motorA.stop();
      motorB.stop();
      vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void test_imu_data(void *pvParameter) {
  while (1) {
    double ax, ay, az, gx, gy, gz, tempC;
    mpu6050.read_data(&ax, &ay, &az, &gx, &gy, &gz, &tempC);
    printf("Accel (g): X=%.2f Y=%.2f Z=%.2f | Gyro (°/s): X=%.2f Y=%.2f Z=%.2f | Temp (°C): T=%.2f\n",
               ax, ay, az, gx, gy, gz, tempC);

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void test_ir_sensor(void *pvParameter) {
  while (1) {
    bool state = ir_center.is_object_detected();
    ESP_LOGI("IR", "Raw IR state: %d", state);
    if (state) {
      ESP_LOGI("IR", "(1) Obstacle DETECTED");
    }
    vTaskDelay(pdMS_TO_TICKS(200));
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

  // IMU data
  /* mpu6050.init_imu(); */

  // IR sensor
  ir_center.init();

  xTaskCreate(test_robot_motion, "robot_movement_task", 2048, NULL, 5, NULL);
  /* xTaskCreate(test_imu_data, "imu_data_task", 2048, NULL, 4, NULL); */
  xTaskCreate(test_ir_sensor, "ir_data_task", 2048, NULL, 3, NULL);



}
