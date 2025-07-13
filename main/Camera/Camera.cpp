
#include "Camera.h"
#include "esp_camera.h"
#include "esp_err.h"

Camera::Camera() {
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;

    config.pin_d0 = CAM_PIN_D0;
    config.pin_d1 = CAM_PIN_D1;
    config.pin_d2 = CAM_PIN_D2;
    config.pin_d3 = CAM_PIN_D3;
    config.pin_d4 = CAM_PIN_D4;
    config.pin_d5 = CAM_PIN_D5;
    config.pin_d6 = CAM_PIN_D6;
    config.pin_d7 = CAM_PIN_D7;

    config.pin_xclk = CAM_PIN_XCLK;
    config.pin_pclk = CAM_PIN_PCLK;
    config.pin_vsync = CAM_PIN_VSYNC;
    config.pin_href = CAM_PIN_HREF;
    config.pin_sccb_sda = CAM_PIN_SIOD;
    config.pin_sccb_scl = CAM_PIN_SIOC;
    config.pin_pwdn = CAM_PIN_PWDN; // Not connected
    config.pin_reset = CAM_PIN_RESET; // Not connected

    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
}

Camera::~Camera() {

}

bool Camera::init() {
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    ESP_LOGE("Camera", "Camera init failed with error 0x%x", err);
    return false;
  }
  ESP_LOGI("Camera", "Camera initialized successfully");
  return true;
}

camera_fb_t* Camera::capture_frame() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    ESP_LOGE("Camera", "Camera capture failed");
    return nullptr;
  }
  ESP_LOGI("Camera", "Captured %zu bytes", fb->len);
  return fb;
}

void Camera::release_frame(camera_fb_t* fb) {
  if (fb) {
    esp_camera_fb_return(fb);
  }
}





