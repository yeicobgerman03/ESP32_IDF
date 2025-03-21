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

// static const char *TAG = "ESP_NOW_Receiver";
// static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // Dirección MAC del emisor

// static esp_err_t init_wifi(void)
// {
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     ESP_ERROR_CHECK(nvs_flash_init());

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
//     ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR)); // Protocolo ESP-NOW
//     ESP_ERROR_CHECK(esp_wifi_start());

//     uint8_t primary_chan;
//     wifi_second_chan_t second_chan;
//     esp_wifi_get_channel(&primary_chan, &second_chan);
//     ESP_LOGI(TAG, "WiFi inicializado en modo STA, canal: %d", primary_chan);

//     return ESP_OK;
// }

// static void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//     ESP_LOGI(TAG, "Estado de envío: %s", status == ESP_NOW_SEND_SUCCESS ? "Éxito" : "Fallo");
// }

// static void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len)
// {
//     ESP_LOGI(TAG, "Mensaje recibido, tamaño: %d", data_len);

//     if (data_len > 0) {
//         char msg[data_len + 1];
//         memcpy(msg, data, data_len);
//         msg[data_len] = '\0';

//         ESP_LOGI(TAG, "Datos recibidos de " MACSTR ": %s", MAC2STR(esp_now_info->src_addr), msg);

//         if (strcmp(msg, "ON") == 0)
//         {
//             gpio_set_level(LED, 1);
//             ESP_LOGI(TAG, "LED ENCENDIDO");
//         }
//         else if (strcmp(msg, "OFF") == 0)
//         {
//             gpio_set_level(LED, 0);
//             ESP_LOGI(TAG, "LED APAGADO");
//         }
//     } else {
//         ESP_LOGW(TAG, "Se recibió un mensaje vacío");
//     }
// }

// static esp_err_t init_esp_now(void)
// {
//     esp_err_t ret = esp_now_init();
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Error al inicializar ESP-NOW: %d", ret);
//         return ret;
//     }

//     esp_now_register_recv_cb(recv_cb);
//     esp_now_register_send_cb(send_cb);
//     ESP_LOGI(TAG, "ESP-NOW inicializado correctamente");

//     return ESP_OK;
// }

// static esp_err_t register_peer(uint8_t *peer_addr)
// {
//     if (!esp_now_is_peer_exist(peer_addr)) {
//         esp_now_peer_info_t peer_info = {};
//         memset(&peer_info, 0, sizeof(esp_now_peer_info_t));
//         memcpy(peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
//         peer_info.channel = ESP_CHANNEL;
//         peer_info.ifidx = WIFI_IF_STA;
//         ESP_ERROR_CHECK(esp_now_add_peer(&peer_info));
//         ESP_LOGI(TAG, "Peer agregado correctamente");
//     } else {
//         ESP_LOGI(TAG, "El peer ya estaba registrado");
//     }

//     return ESP_OK;
// }

// void app_main(void)
// {
//     ESP_ERROR_CHECK(init_wifi());
//     ESP_ERROR_CHECK(init_esp_now());
//     ESP_ERROR_CHECK(register_peer(peer_mac));

//     gpio_reset_pin(LED);
//     gpio_set_direction(LED, GPIO_MODE_OUTPUT);

//     ESP_LOGI(TAG, "ESP32 receptor listo para recibir datos");
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CON LED
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

// static const char *TAG = "ESP_NOW_Receptor";
// static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// static esp_err_t init_wifi(void)
// {
//     esp_netif_init();
//     esp_event_loop_create_default();
//     nvs_flash_init();

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     esp_wifi_init(&cfg);
//     esp_wifi_set_mode(WIFI_MODE_STA);
//     esp_wifi_start();

//     ESP_LOGI(TAG, "WiFi inicializado en modo STA");
//     return ESP_OK;
// }

// static void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//     ESP_LOGI(TAG, "Estado de envío: %s", status == ESP_NOW_SEND_SUCCESS ? "Éxito" : "Fallo");
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

// static void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len)
// {
//     char msg[data_len + 1];
//     memcpy(msg, data, data_len);
//     msg[data_len] = '\0';

//     ESP_LOGI(TAG, "Datos recibidos de " MACSTR ": %s", MAC2STR(esp_now_info->src_addr), msg);

//     if (strcmp(msg, "BLINK") == 0)
//     {
//         ESP_LOGI(TAG, "Iniciando parpadeo");
//         blink_led();
//     }
// }

// static esp_err_t init_esp_now(void)
// {
//     esp_err_t ret = esp_now_init();
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Error al inicializar ESP-NOW: %d", ret);
//         return ret;
//     }

//     esp_now_register_recv_cb(recv_cb);
//     esp_now_register_send_cb(send_cb);
//     ESP_LOGI(TAG, "ESP-NOW inicializado correctamente");

//     return ESP_OK;
// }

// static esp_err_t register_peer(uint8_t *peer_addr)
// {
//     if (!esp_now_is_peer_exist(peer_addr))
//     {
//         esp_now_peer_info_t peer_info = {};
//         memcpy(peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
//         peer_info.channel = ESP_CHANNEL;
//         peer_info.ifidx = WIFI_IF_STA;
//         ESP_ERROR_CHECK(esp_now_add_peer(&peer_info));
//         ESP_LOGI(TAG, "Peer agregado correctamente");
//     }

//     return ESP_OK;
// }

// void app_main(void)
// {
//     ESP_ERROR_CHECK(init_wifi());
//     ESP_ERROR_CHECK(init_esp_now());
//     ESP_ERROR_CHECK(register_peer(peer_mac));

//     gpio_reset_pin(LED);
//     gpio_set_direction(LED, GPIO_MODE_OUTPUT);

//     ESP_LOGI(TAG, "ESP32 receptor listo para recibir datos");
// }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CON PWM

// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_now.h"
// #include "esp_wifi.h"
// #include "nvs_flash.h"
// #include "driver/ledc.h"
// #include "esp_log.h"

// #define LED_PIN 33
// #define LEDC_TIMER LEDC_TIMER_0
// #define LEDC_MODE LEDC_HIGH_SPEED_MODE
// #define LEDC_CHANNEL LEDC_CHANNEL_0
// #define LEDC_RESOLUTION LEDC_TIMER_10_BIT
// #define LEDC_FREQUENCY 5000

// typedef enum {
//     STATE_RECEIVE_DATA,
//     STATE_UPDATE_PWM
// } state_t;

// typedef struct {
//     uint8_t duty_cycle;
// } espnow_data_t;

// static state_t current_state = STATE_RECEIVE_DATA;
// static uint8_t received_duty_cycle = 0;

// void espnow_recv_cb(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
//     espnow_data_t *received_data = (espnow_data_t *)data;
//     received_duty_cycle = received_data->duty_cycle;
//     current_state = STATE_UPDATE_PWM;
// }

// void pwm_control_task(void *pvParameter) {
//     ledc_timer_config_t ledc_timer = {
//         .speed_mode = LEDC_MODE,
//         .timer_num = LEDC_TIMER,
//         .duty_resolution = LEDC_RESOLUTION,
//         .freq_hz = LEDC_FREQUENCY,
//         .clk_cfg = LEDC_AUTO_CLK
//     };
//     ledc_timer_config(&ledc_timer);

//     ledc_channel_config_t ledc_channel = {
//         .speed_mode = LEDC_MODE,
//         .channel = LEDC_CHANNEL,
//         .timer_sel = LEDC_TIMER,
//         .intr_type = LEDC_INTR_DISABLE,
//         .gpio_num = LED_PIN,
//         .duty = 0,
//         .hpoint = 0
//     };
//     ledc_channel_config(&ledc_channel);

//     while (1) {
//         switch (current_state) {
//             case STATE_RECEIVE_DATA:
//                 // Esperando datos
//                 break;

//             case STATE_UPDATE_PWM:
//                 uint32_t duty = (received_duty_cycle * 1023) / 100;
//                 ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
//                 ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
//                 ESP_LOGI("PWM", "Updated duty cycle: %d%%", received_duty_cycle);
//                 current_state = STATE_RECEIVE_DATA;
//                 break;
//         }
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

// void app_main() {
//     nvs_flash_init();
//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     esp_wifi_init(&cfg);
//     esp_wifi_set_mode(WIFI_MODE_STA);
//     esp_wifi_start();

//     esp_now_init();
//     esp_now_register_recv_cb(espnow_recv_cb);

//     xTaskCreate(pwm_control_task, "pwm_control_task", 2048, NULL, 1, NULL);
// }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//obtener direccion mac

// #include <stdio.h>
// #include "esp_system.h"
// #include "esp_mac.h"

// void app_main()
// {
//     uint8_t mac[6];
//     esp_err_t ret = esp_base_mac_addr_get(mac);

//     if (ret == ESP_OK)
//     {
//         printf("Dirección MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
//                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
//     }
//     else
//     {
//         printf("Error obteniendo la MAC: %d\n", ret);
//     }
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODIGO REAL

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

#define LED_PIN 33
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
