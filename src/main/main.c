#include "espnow_sender.h"
#include "nvs_flash.h"


void app_main()
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    espnow_wifi_init();

    // Loop principal
    while (1)
    {
        send_espnow(123456654, 100,455);
        // Aguardar antes de enviar novamente (ajuste conforme necessário)
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}
