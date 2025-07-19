#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <stdio.h>
#include <string>
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "esp_log.h"


class UDPServer {
public:
  UDPServer();
  bool begin(uint16_t listen_port);           // Create socket, bind it to the port and prepare for receiving
  int receive(char* buffer, size_t length);
  int send(const char* ip, uint16_t port, const char* data, size_t length);

private:
  int sock;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

};

#endif  // UDP_SERVER_H
