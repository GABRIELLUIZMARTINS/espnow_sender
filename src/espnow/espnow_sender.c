#include "espnow_sender.h"

// Função de inicialização do WiFi
esp_err_t wifi_init()
{
    esp_err_t ret_code = ESP_FAIL;
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ret_code = esp_wifi_init(&cfg);
    ESP_ERROR_CHECK(ret_code);
    ret_code = esp_wifi_set_storage(WIFI_STORAGE_RAM);
    ESP_ERROR_CHECK(ret_code);
    ret_code = esp_wifi_start();
    ESP_ERROR_CHECK(ret_code);
    ret_code = esp_wifi_set_mode(WIFI_MODE_STA);
    ESP_ERROR_CHECK(ret_code);
    return ret_code;
}

esp_err_t espnow_wifi_init()
{
    esp_err_t ret_code = ESP_FAIL;
    uint8_t broadcast_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // Endereço MAC de broadcast

    // Inicializa wifi
    ret_code = wifi_init();
    ESP_ERROR_CHECK(ret_code);

    // Inicializa o ESP-NOW
    ret_code = esp_now_init();
    ESP_ERROR_CHECK(ret_code);

    // Adicionar o endereço de broadcast como peer
    esp_now_peer_info_t peer_info;
    memset(&peer_info, 0, sizeof(esp_now_peer_info_t));
    memcpy(peer_info.peer_addr, broadcast_addr, ESP_NOW_ETH_ALEN);
    peer_info.channel = 1;  // Pode ajustar o canal conforme necessário
    peer_info.ifidx = ESP_IF_WIFI_STA;
    peer_info.encrypt = false;  // Desativar a criptografia, se necessário

    esp_now_add_peer(&peer_info);
    ESP_ERROR_CHECK(ret_code);
    return ret_code;
}

esp_err_t send_espnow(uint32_t signal_v, uint32_t baseline_v, uint32_t threshold_v)
{
    // Endereço MAC de broadcast
    uint8_t broadcast_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static const char *TAG = "espnow_sender";

    Data_t dados_enviar = {.signal = signal_v, .baseline = baseline_v, .threshold = threshold_v};

    // Enviar a estrutura via ESP-NOW para o endereço de broadcast
    esp_err_t result = esp_now_send(broadcast_addr, (uint8_t *)&dados_enviar, sizeof(Data_t));
    if (result == ESP_OK)
    {
        ESP_LOGI(TAG, "Dados enviados com sucesso para broadcast");
    }
    else
    {
        ESP_LOGE(TAG, "Erro ao enviar dados: %s", esp_err_to_name(result));
    }
    return result;
}
