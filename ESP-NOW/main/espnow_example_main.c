// #include <stdio.h>
// #include "string.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_now.h"
// #include "esp_wifi.h"
// #include "esp_netif.h"
// #include "esp_mac.h"
// #include "esp_event.h"
// #include "nvs_flash.h"
// #include "esp_log.h"
// #include "driver/gpio.h"

// #define ESP_CHANNEL 1
// #define LED 2

// static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
// static const char *TAG = "esp_now_init";

// static esp_err_t init_wifi(void)
// {
//     wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

//     esp_netif_init();
//     esp_event_loop_create_default();
//     nvs_flash_init();
//     esp_wifi_init(&wifi_init_config);
//     esp_wifi_set_mode(WIFI_MODE_STA);
//     esp_wifi_set_storage(WIFI_STORAGE_FLASH);
//     esp_wifi_start();
//     ESP_LOGI(TAG, "WiFi init completed");
//     return ESP_OK;
// }

// void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len)
// {
//     ESP_LOGI(TAG, "Data received from " MACSTR ": %s", MAC2STR(esp_now_info->src_addr), data);
// }

// void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//     if (status == ESP_NOW_SEND_SUCCESS)
//     {
//         ESP_LOGI(TAG, "ESP_NOW_SEND_SUCCESS");
//     }
//     else
//     {
//         ESP_LOGW(TAG, "ESP_NOW_SEND_FAIL");
//     }
// }

// static esp_err_t init_esp_now(void)
// {
//     esp_err_t ret = esp_now_init();
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "ESP-NOW no pudo inicializarse, error: %d", ret);
//         return ret;
//     }

//     esp_now_register_recv_cb(recv_cb);
//     esp_now_register_send_cb(send_cb);
//     ESP_LOGI(TAG, "ESP-NOW inicializado correctamente");
//     return ESP_OK;
// }

// static esp_err_t register_peer(uint8_t *peer_addr)
// {
//     esp_now_peer_info_t esp_now_peer_info = {};
//     memcpy(esp_now_peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
//     esp_now_peer_info.channel = ESP_CHANNEL;
//     esp_now_peer_info.ifidx = WIFI_IF_STA;

//     esp_err_t result = esp_now_add_peer(&esp_now_peer_info);
//     if (result == ESP_OK) {
//         ESP_LOGI(TAG, "Peer agregado correctamente");
//     } else {
//         ESP_LOGE(TAG, "Error al agregar peer: %d", result);
//     }
//     return result;
// }

// static esp_err_t esp_now_send_data(const uint8_t *peer_addr, const char *data)
// {
//     return esp_now_send(peer_addr, (uint8_t *)data, strlen(data) + 1);
// }

// static void send_task(void *pvParameter)
// {
//     while (1)
//     {
//         const char *msg = "Hello ESP-NOW";
//         esp_err_t result = esp_now_send(peer_mac, (uint8_t *)msg, strlen(msg) + 1);

//         if (result == ESP_OK) {
//             ESP_LOGI(TAG, "Mensaje enviado: %s", msg);
//         } else {
//             ESP_LOGE(TAG, "Error al enviar mensaje: %d", result);
//         }

//         vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo
//     }
// }

// void app_main(void)
// {
//     ESP_ERROR_CHECK(init_wifi());
//     ESP_ERROR_CHECK(init_esp_now());
//     ESP_ERROR_CHECK(register_peer(peer_mac));

//     xTaskCreate(&send_task, "send_task", 2048, NULL, 5, NULL);
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CON LED//
// #include <stdio.h>
// #include "string.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_now.h"
// #include "esp_wifi.h"
// #include "esp_netif.h"
// #include "esp_mac.h"
// #include "esp_event.h"
// #include "nvs_flash.h"
// #include "esp_log.h"
// #include "driver/gpio.h"

// #define ESP_CHANNEL 1
// #define LED 2

// static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0x3c, 0x8a, 0x1f, 0xa8, 0x8b, 0x7c};
// static const char *TAG = "ESP_NOW_Emisor";

// static esp_err_t init_wifi(void)
// {
//     wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
//     esp_netif_init();
//     esp_event_loop_create_default();
//     nvs_flash_init();
//     esp_wifi_init(&wifi_init_config);
//     esp_wifi_set_mode(WIFI_MODE_STA);
//     esp_wifi_set_storage(WIFI_STORAGE_FLASH);
//     esp_wifi_start();
//     ESP_LOGI(TAG, "WiFi inicializado");
//     return ESP_OK;
// }

// void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//     ESP_LOGI(TAG, "Estado de envío: %s", status == ESP_NOW_SEND_SUCCESS ? "Éxito" : "Fallo");
// }

// static esp_err_t init_esp_now(void)
// {
//     esp_err_t ret = esp_now_init();
//     if (ret != ESP_OK)
//     {
//         ESP_LOGE(TAG, "ESP-NOW no pudo inicializarse, error: %d", ret);
//         return ret;
//     }

//     esp_now_register_send_cb(send_cb);
//     ESP_LOGI(TAG, "ESP-NOW inicializado");
//     return ESP_OK;
// }

// static esp_err_t register_peer(uint8_t *peer_addr)
// {
//     esp_now_peer_info_t peer_info = {};
//     memcpy(peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
//     peer_info.channel = ESP_CHANNEL;
//     peer_info.ifidx = WIFI_IF_STA;

//     esp_err_t result = esp_now_add_peer(&peer_info);
//     if (result == ESP_OK)
//     {
//         ESP_LOGI(TAG, "Peer agregado correctamente");
//     }
//     else
//     {
//         ESP_LOGE(TAG, "Error al agregar peer: %d", result);
//     }
//     return result;
// }

// static void blink_led()
// {
//     for (int i = 0; i < 1; i++)
//     {
//         gpio_set_level(LED, 1);
//         vTaskDelay(pdMS_TO_TICKS(500));
//         gpio_set_level(LED, 0);
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }

// static void send_task(void *pvParameter)
// {
//     gpio_reset_pin(LED);
//     gpio_set_direction(LED, GPIO_MODE_OUTPUT);

//     while (1)
//     {
//         const char *msg = "BLINK";
//         esp_now_send(peer_mac, (uint8_t *)msg, strlen(msg) + 1);
//         ESP_LOGI(TAG, "Mensaje enviado: %s", msg);

//         // Parpadear LED localmente
//         blink_led();

//         vTaskDelay(pdMS_TO_TICKS(50)); // Espera 5 segundos antes de enviar de nuevo
//     }
// }

// void app_main(void)
// {
//     ESP_ERROR_CHECK(init_wifi());
//     ESP_ERROR_CHECK(init_esp_now());
//     ESP_ERROR_CHECK(register_peer(peer_mac));

//     xTaskCreate(&send_task, "send_task", 2048, NULL, 5, NULL);
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////

/* ==========================
 * Código del Emisor ESP32
 * ========================== */

//  #include <stdio.h>
//  #include <string.h>
//  #include "freertos/FreeRTOS.h"
//  #include "freertos/task.h"
//  #include "freertos/queue.h"
//  #include "esp_now.h"
//  #include "esp_wifi.h"
//  #include "nvs_flash.h"
//  #include "driver/adc.h"
//  #include "esp_log.h"

//  #define POT_PIN ADC1_CHANNEL_4 // GPIO 32

//  typedef enum {
//      STATE_READ_ADC,
//      STATE_SEND_DATA
//  } state_t;

//  typedef struct {
//      uint8_t duty_cycle;
//  } espnow_data_t;

//  static uint8_t peer_mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // MAC del receptor
//  static state_t current_state = STATE_READ_ADC;

//  void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
//      ESP_LOGI("ESP-NOW", "Send status: %s", status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
//  }

//  void send_task(void *pvParameter) {
//      espnow_data_t data;
//      while (1) {
//          switch (current_state) {
//              case STATE_READ_ADC:
//                  // Leer el ADC y convertir a porcentaje de duty cycle (0-100)
//                  int adc_reading = adc1_get_raw(POT_PIN);
//                  data.duty_cycle = (adc_reading * 100) / 4095;
//                  current_state = STATE_SEND_DATA;
//                  break;

//              case STATE_SEND_DATA:
//                  esp_now_send(peer_mac, (uint8_t *)&data, sizeof(data));
//                  current_state = STATE_READ_ADC;
//                  break;
//          }
//          vTaskDelay(pdMS_TO_TICKS(100));
//      }
//  }

//  void app_main() {
//      nvs_flash_init();
//      wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//      esp_wifi_init(&cfg);
//      esp_wifi_set_mode(WIFI_MODE_STA);
//      esp_wifi_start();

//      esp_now_init();
//      esp_now_register_send_cb(espnow_send_cb);

//      esp_now_peer_info_t peer_info = {};
//      memcpy(peer_info.peer_addr, peer_mac, 6);
//      peer_info.channel = 0;
//      peer_info.encrypt = false;
//      esp_now_add_peer(&peer_info);

//      adc1_config_width(ADC_WIDTH_BIT_12);
//      adc1_config_channel_atten(POT_PIN, ADC_ATTEN_DB_12);

//      xTaskCreate(send_task, "send_task", 2048, NULL, 1, NULL);
//  }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODIGO REAL+
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "esp_event.h"

#define ESP_CHANNEL 0
#define POT_PIN ADC1_CHANNEL_4                                    // GPIO 32
static uint8_t peer_mac[] = {0x3C, 0x8A, 0x1F, 0xA8, 0x8B, 0x7C}; // MAC del receptor

typedef enum
{
    STATE_READ_ADC,
    STATE_SEND_DATA
} state_t;

typedef struct
{
    uint8_t duty_cycle;
} espnow_data_t;

static state_t current_state = STATE_READ_ADC;
static const char *TAG = "ESP_NOW";

static esp_err_t wifi_init(void)
{
    ESP_ERROR_CHECK(nvs_flash_init()); // Inicializar NVS

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi inicializado correctamente");
    return ESP_OK;
}

void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    ESP_LOGI(TAG, "Estado de envío: %s", status == ESP_NOW_SEND_SUCCESS ? "Éxito" : "Fallo");
}

static esp_err_t init_esp_now(void)
{
    if (esp_now_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al inicializar ESP-NOW");
        return ESP_FAIL;
    }

    ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_send_cb));
    ESP_LOGI(TAG, "ESP-NOW inicializado correctamente");
    return ESP_OK;
}

static esp_err_t register_peer(void)
{
    esp_now_peer_info_t peer_info = {0};
    memcpy(peer_info.peer_addr, peer_mac, 6);
    peer_info.channel = ESP_CHANNEL;
    peer_info.encrypt = false;
    peer_info.ifidx = ESP_IF_WIFI_STA;

    if (esp_now_add_peer(&peer_info) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error al registrar el peer");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Peer registrado correctamente");
    return ESP_OK;
}

void send_task(void *pvParameter)
{
    espnow_data_t data;
    while (1)
    {
        switch (current_state)
        {
        case STATE_READ_ADC:
            // Leer el ADC y convertir a porcentaje de duty cycle (0-100)
            data.duty_cycle = (adc1_get_raw(POT_PIN) * 100) / 4095;
            current_state = STATE_SEND_DATA;
            break;

        case STATE_SEND_DATA:
            if (esp_now_send(peer_mac, (uint8_t *)&data, sizeof(data)) != ESP_OK)
            {
                ESP_LOGE(TAG, "Error al enviar datos por ESP-NOW");
            }
            current_state = STATE_READ_ADC;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main()
{
    ESP_ERROR_CHECK(wifi_init());
    ESP_ERROR_CHECK(init_esp_now());
    ESP_ERROR_CHECK(register_peer());

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(POT_PIN, ADC_ATTEN_DB_12);

    xTaskCreate(send_task, "send_task", 2048, NULL, 1, NULL);
}
