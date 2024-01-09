#include "espnow_sender.h"

// WiFi initialization function
esp_err_t wifi_init()
{
    esp_err_t ret_code = ESP_FAIL;
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    // Init wifi to espnow
    ret_code = esp_wifi_init(&cfg);
    ESP_ERROR_CHECK(ret_code);

    // Configure RAM storage mode
    ret_code = esp_wifi_set_storage(WIFI_STORAGE_RAM);
    ESP_ERROR_CHECK(ret_code);

    // Station mode (aka STA mode or Wi-Fi client mode). ESP32 connects to an access point.
    ret_code = esp_wifi_set_mode(WIFI_MODE_STA);
    ESP_ERROR_CHECK(ret_code);
    // Sets wifi power mode to sleep (For maximum savings)
    ret_code = esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
    ESP_ERROR_CHECK(ret_code);

    // Start wifi
    ret_code = esp_wifi_start();
    ESP_ERROR_CHECK(ret_code);
    return ret_code;
}

esp_err_t espnow_wifi_init()
{
    esp_err_t ret_code = ESP_FAIL;
    uint8_t broadcast_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // Endereço MAC de broadcast

    // Initialization wifi
    ret_code = wifi_init();
    ESP_ERROR_CHECK(ret_code);

    // Initialization ESP-NOW
    ret_code = esp_now_init();
    ESP_ERROR_CHECK(ret_code);

    // Add broadcast address as a peer
    esp_now_peer_info_t peer_info;
    memset(&peer_info, 0, sizeof(esp_now_peer_info_t));
    memcpy(peer_info.peer_addr, broadcast_addr, ESP_NOW_ETH_ALEN);
    peer_info.channel = 1;
    peer_info.ifidx = ESP_IF_WIFI_STA;
    peer_info.encrypt = false;  // Disable encryption

    esp_now_add_peer(&peer_info);
    ESP_ERROR_CHECK(ret_code);
    return ret_code;
}

esp_err_t send_espnow(uint32_t signal_v, uint32_t baseline_v, uint32_t threshold_v)
{
    // MAC broadcast address
    uint8_t broadcast_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static const char *TAG = "espnow_sender";

    Data_t data = {.signal = signal_v, .baseline = baseline_v, .threshold = threshold_v};

    // Send the structure via ESP-NOW to the broadcast address
    esp_err_t result = esp_now_send(broadcast_addr, (uint8_t *)&data, sizeof(Data_t));
    if (result == ESP_OK)
    {
        ESP_LOGI(TAG, "Data successfully send to broadcast.");
    }
    else
    {
        ESP_LOGE(TAG, "Error sending data: %s", esp_err_to_name(result));
    }
    return result;
}
