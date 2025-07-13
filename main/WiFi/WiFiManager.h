#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <stdio.h>
#include "string.h"

#include "esp_err.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_wifi_default.h"
#include "esp_wifi_types_generic.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#define MAX_STA_CONN 1
#define WIFI_CHANNEL 1

class WiFiManager {
public:
  WiFiManager() {}
  bool init_AP(const char* ssid, const char* password);

};

#endif   // WIFI_MANAGER_H
