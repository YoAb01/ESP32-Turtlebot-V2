#include "WiFi/WiFiManager.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "nvs_flash.h"


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
