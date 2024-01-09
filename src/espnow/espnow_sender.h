#ifndef ESPNOW_SENDER_H
#define ESPNOW_SENDER_H

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

// Estrutura para armazenar os dados a serem enviados
typedef struct
{
    uint32_t signal;
    uint32_t baseline;
    uint32_t threshold;
} Data_t;

static const char *TAG = "espnow_sender";

void send_espnow(uint32_t signal_v,uint32_t baseline_v,uint32_t threshold_v);
void espnow_wifi_init();

#endif