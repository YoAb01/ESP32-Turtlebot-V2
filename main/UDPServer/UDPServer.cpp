#include "UDPServer/UDPServer.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include <cerrno>
#include <cstddef>
#include <iterator>


bool UDPServer::begin(uint16_t listen_port) {
  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    ESP_LOGE("UDPServer", "Unable to create socket: errno %d", errno);
    return false;
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(listen_port);

  int _bind = bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (_bind < 0) {
    ESP_LOGE("UDPServer", "Socket unable to bind: errno %d", errno);
    close(sock);
    return false;
  }
  ESP_LOGI("UDPServer", "UDP server listening on port %d", listen_port);
  return true;
}

int UDPServer::receive(char* buffer, size_t length) {
  socklen_t socklen = sizeof(client_addr);
  int len = recvfrom(sock, buffer, length, 0, (struct sockaddr *)&client_addr, &socklen);
  if (len < 0) {
    ESP_LOGE("UDPSever", "recvfrom failed: errno %d", errno);
    return -1;
  }
  return len;
}

int UDPServer::send(const char* ip, uint16_t port, const char* data, size_t length) {
  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(port);
  dest_addr.sin_addr.s_addr = inet_addr(ip);

  if (dest_addr.sin_addr.s_addr == INADDR_NONE) {
    ESP_LOGE("UDPServer", "Invalid IP address");
    return -1;
  }

  int s = sendto(sock, data, length, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
  if (s < 0) {
    ESP_LOGE("UDPSever", "send failed: errno %d", errno);
    return -1;
  }
  ESP_LOGI("UDPServer", "Sent %d bytes to %s:%d", s, ip, port);
  return s;
}
