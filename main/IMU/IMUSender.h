#ifndef IMU_SENDER_H
#define IMU_SENDER_H

#include "IMU.h"
#include "lwip/sockets.h"
#include "esp_log.h"
#include <cstdint>
#include <cstring>

// Simple packet structure - matches what Qt will receive
struct IMUPacket {
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
    float temp;
    uint32_t timestamp_ms;
} __attribute__((packed));


class IMUSender {
public:
  IMUSender(const char* target_ip, uint16_t target_port) : _target_port(target_port), _sock(-1) {
    strncpy(_target_ip, target_ip, sizeof(_target_ip) - 1);
  }

  ~IMUSender() {
    if (_sock >= 0) {
      close(_sock);
    }
  }

  bool init();

  void send_imu_data(double ax, double ay, double az, double gx, double gy, double gz, double temp);


private:
    char _target_ip[16];
    uint16_t _target_port;
    int _sock;
    struct sockaddr_in _dest_addr;
};


#endif    // IMU_SENDER_H
