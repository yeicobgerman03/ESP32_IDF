#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "driver/gpio.h"
#include "cJSON.h" // Asegúrate de incluir la biblioteca cJSON


static const char *TAG = "mqttws_example";

#define LED_GPIO_PIN 2 // Define el pin GPIO donde está conectado el LED

typedef enum
{
    LED_OFF,
    LED_ON
} led_state_t;

led_state_t led_state = LED_OFF;

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0)
    {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static void mqtt_event_handler(void *handler_args,
                               esp_event_base_t base,
                               int32_t event_id,
                               void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

        msg_id = esp_mqtt_client_subscribe(client, "yeicob/g", 0); // Suscríbete al tópico
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);

        // Extraer el valor del campo "msg" del JSON recibido
        if (event->data_len > 0)
        {
            cJSON *root = cJSON_Parse(event->data);
            if (root != NULL)
            {
                cJSON *msg = cJSON_GetObjectItem(root, "msg");
                if (cJSON_IsString(msg) && (msg->valuestring != NULL))
                {
                    // Si msg es "1", enciende el LED
                    if (strcmp(msg->valuestring, "1") == 0 && led_state == LED_OFF)
                    {
                        ESP_LOGI(TAG, "Turning LED ON");
                        gpio_set_level(LED_GPIO_PIN, 1); // Enciende el LED
                        led_state = LED_ON;
                    }
                    // Si msg es "0", apaga el LED
                    else if (strcmp(msg->valuestring, "0") == 0 && led_state == LED_ON)
                    {
                        ESP_LOGI(TAG, "Turning LED OFF");
                        gpio_set_level(LED_GPIO_PIN, 0); // Apaga el LED
                        led_state = LED_OFF;
                    }
                }
                cJSON_Delete(root);
            }
        }
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;

    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

static void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "ws://broker.emqx.io:8083/mqtt",
        .credentials.username = "Yeicob",
        .credentials.authentication.password = "12345678"};

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("mqtt_example", ESP_LOG_VERBOSE);
    esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
    esp_log_level_set("transport_ws", ESP_LOG_VERBOSE);
    esp_log_level_set("transport", ESP_LOG_VERBOSE);
    esp_log_level_set("outbox", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());

    // Configura el GPIO 2 como salida para el LED
    gpio_set_direction(LED_GPIO_PIN, GPIO_MODE_OUTPUT);

    // Inicia el cliente MQTT
    mqtt_app_start();
}
