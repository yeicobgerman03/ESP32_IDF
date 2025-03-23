#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_event.h"

#define LED_PIN 26
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_RESOLUTION LEDC_TIMER_10_BIT
#define LEDC_FREQUENCY 5000

static const char *TAG = "ESP_NOW_RESPONDER";

typedef enum
{
    STATE_RECEIVE_DATA,
    STATE_UPDATE_PWM
} state_t;

typedef struct
{
    uint8_t duty_cycle;
} espnow_data_t;

static state_t current_state = STATE_RECEIVE_DATA;
static uint8_t received_duty_cycle = 0;

static esp_err_t wifi_init(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi inicializado correctamente");
    return ESP_OK;
}

void espnow_recv_cb(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
    if (len != sizeof(espnow_data_t))
    {
        ESP_LOGW(TAG, "Tamaño de datos incorrecto: esperado %d, recibido %d", sizeof(espnow_data_t), len);
        return;
    }

    espnow_data_t *received_data = (espnow_data_t *)data;
    received_duty_cycle = received_data->duty_cycle;
    current_state = STATE_UPDATE_PWM;
}

static esp_err_t init_esp_now(void)
{
    if (esp_now_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al inicializar ESP-NOW");
        return ESP_FAIL;
    }

    ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_recv_cb));
    ESP_LOGI(TAG, "ESP-NOW inicializado correctamente");
    return ESP_OK;
}

void pwm_control_task(void *pvParameter)
{
    // Configurar el timer del LEDC
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_RESOLUTION,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Configurar el canal del LEDC
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LED_PIN,
        .duty = 0,
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    while (1)
    {
        switch (current_state)
        {
        case STATE_RECEIVE_DATA:
            // Esperando datos
            break;

        case STATE_UPDATE_PWM:
            // Convertir el porcentaje a resolución de 10 bits (0-1023)
            uint32_t duty = (received_duty_cycle * 1023) / 100;
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));

            ESP_LOGI(TAG, "Ciclo de trabajo actualizado: %d%%", received_duty_cycle);
            current_state = STATE_RECEIVE_DATA;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main()
{
    ESP_ERROR_CHECK(wifi_init());
    ESP_ERROR_CHECK(init_esp_now());

    xTaskCreate(pwm_control_task, "pwm_control_task", 2048, NULL, 1, NULL);
}

