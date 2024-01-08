#include <stdio.h>
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include <string.h> 

// Endereço MAC de broadcast
uint8_t broadcast_addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Estrutura para armazenar os dados a serem enviados
typedef struct {
    int valor1;
    int valor2;
    int valor3;
} Dados;

static const char *TAG = "espnow_sender";

// Função de inicialização do WiFi
static void wifi_init() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main() {

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    wifi_init();

    // Configurar ESP-NOW
    ESP_ERROR_CHECK(esp_now_init());
    
    // Adicionar o endereço de broadcast como peer
    esp_now_peer_info_t peer_info;
    memset(&peer_info, 0, sizeof(esp_now_peer_info_t));
    memcpy(peer_info.peer_addr, broadcast_addr, ESP_NOW_ETH_ALEN);
    peer_info.channel = 1;  // Pode ajustar o canal conforme necessário
    peer_info.ifidx = ESP_IF_WIFI_STA;
    peer_info.encrypt = false;  // Desativar a criptografia, se necessário
    ESP_ERROR_CHECK(esp_now_add_peer(&peer_info));

    // Loop principal
    while (1) {
        // Criar e preencher a estrutura com dados fictícios
        Dados dados_enviar = {
            .valor1 = 42,
            .valor2 = -123,
            .valor3 = 987
        };

        // Enviar a estrutura via ESP-NOW para o endereço de broadcast
        esp_err_t result = esp_now_send(broadcast_addr, (uint8_t*)&dados_enviar, sizeof(Dados));
        if (result == ESP_OK) {
            ESP_LOGI(TAG, "Dados enviados com sucesso para broadcast");
        } else {
            ESP_LOGE(TAG, "Erro ao enviar dados: %s", esp_err_to_name(result));
        }

        // Aguardar antes de enviar novamente (ajuste conforme necessário)
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}
