#include "IMUSender.h"
#include "lwip/sockets.h"


bool IMUSender::init() {
  // Create UDP socket
  _sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (_sock < 0) {
    ESP_LOGE("IMUSender", "Failed to create socket!");
    return false;
  }

  // Setup destination
  memset(&_dest_addr, 0, sizeof(_dest_addr));
  _dest_addr.sin_family = AF_INET;
  _dest_addr.sin_port = htons(_target_port);
  inet_pton(AF_INET, _target_ip, &_dest_addr.sin_addr);

  ESP_LOGI("IMUSender", "UDP sender initialized: %s:%d", _target_ip, _target_port);
  return true;
}

void IMUSender::send_imu_data(double ax, double ay, double az, double gx, double gy, double gz, double temp) {
  if (_sock < 0) {
    return;
  }

  // Pack data into packet
  IMUPacket packet;
  packet.accel_x = (float)ax;
  packet.accel_y = (float)ay;
  packet.accel_z = (float)az;
  packet.gyro_x = (float)gx;
  packet.gyro_y = (float)gy;
  packet.gyro_z = (float)gz;
  packet.temp = (float)temp;
  packet.timestamp_ms = esp_log_timestamp();

  // Send packet
  int sent = sendto(_sock, &packet, sizeof(packet), 0, (struct sockaddr*)&_dest_addr, sizeof(_dest_addr));

  if (sent < 0) {
    ESP_LOGW("IMUSender", "Failed to send packet");
  }
}
