#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <cstdint>
#include "driver/gptimer_types.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "soc/gpio_num.h"
#include "esp_log.h"
#include "esp_camera.h"

#define CAM_PIN_PWDN -1
#define CAM_PIN_RESET -1
#define CAM_PIN_VSYNC 6
#define CAM_PIN_HREF 7
#define CAM_PIN_PCLK 13
#define CAM_PIN_XCLK 15
#define CAM_PIN_SIOD 4
#define CAM_PIN_SIOC 5
#define CAM_PIN_D0 11
#define CAM_PIN_D1 9
#define CAM_PIN_D2 8
#define CAM_PIN_D3 10
#define CAM_PIN_D4 12
#define CAM_PIN_D5 18
#define CAM_PIN_D6 17
#define CAM_PIN_D7 16

class Camera {
public:
  Camera();
  ~Camera();

  bool init();
  camera_fb_t* capture_frame();
  void release_frame(camera_fb_t *fb);

private:
  camera_config_t config;

};

#endif   // CAMERA_H
