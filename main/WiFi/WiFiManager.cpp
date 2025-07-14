#include "WiFi/WiFiManager.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_wifi_types_generic.h"
#include "nvs_flash.h"

std::string WiFiManager::_local_ip;

bool WiFiManager::init_AP(const char* ssid, const char* password) {
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  esp_netif_create_default_wifi_ap();

  wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
  
  wifi_config_t ap_config = {};
  strcpy((char *)ap_config.ap.ssid, ssid);
  strcpy((char *)ap_config.ap.password, password);
  ap_config.ap.ssid_len = strlen((char *)ap_config.ap.ssid);
  ap_config.ap.max_connection = MAX_STA_CONN;
  ap_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
  ap_config.ap.channel = WIFI_CHANNEL;

  if (strlen((char *)ap_config.ap.password) == 0) {
    ap_config.ap.authmode = WIFI_AUTH_OPEN;
  }
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI("WIFI_AP", "Access Point started! SSID: %s, Password: %s", ap_config.ap.ssid, ap_config.ap.password);
  return true;
}


void WiFiManager::wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    ESP_LOGI("WIFI_STA", "Disconnected. Reconnecting...");
    esp_wifi_connect();
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    char ip_str[16];
    sprintf(ip_str, IPSTR, IP2STR(&event->ip_info.ip));
    _local_ip = std::string(ip_str);
    ESP_LOGI("WIFI_STA", "Got IP Address: %s", ip_str);
  }
}

bool WiFiManager::init_STA(const char* ssid, const char* password) {
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  esp_netif_create_default_wifi_sta();

  wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));

  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

  wifi_config_t sta_config = {};
  strcpy((char *)sta_config.sta.ssid, ssid);
  strcpy((char *)sta_config.sta.password, password);
  sta_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
  sta_config.sta.pmf_cfg.capable = true;
  sta_config.sta.pmf_cfg.required = false;

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI("WIFI_STA", "wifi_init_sta finished");
  ESP_LOGI("WIFI_STA", "Connecting to %s...", ssid);
  return true;
}

std::string WiFiManager::get_local_ip() {
  return _local_ip;
}
