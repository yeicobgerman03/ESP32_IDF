#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "lwip/apps/sntp.h"
#include "driver/uart.h"

#define WIFI_SSID "Wind_4G_LTE_CC0"
#define WIFI_PASS "federico1144"

static const char *TAG = "NTP";
static EventGroupHandle_t s_wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0

#define UART_NUM UART_NUM_2 // UART2 para Nextion
#define TXD_PIN 17          // GPIO para TX
#define RXD_PIN 16          // GPIO para RX (No lo usamos en este caso)
#define BAUD_RATE 9600      // Velocidad de comunicación con Nextion

// Manejador de eventos WiFi
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGE(TAG, "Conexión perdida. Reintentando...");
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init()
{
    esp_netif_init();
    esp_event_loop_create_default();
    nvs_flash_init();

    s_wifi_event_group = xEventGroupCreate(); // Crear grupo de eventos

    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "Conectando a WiFi...");

    // Espera hasta que se conecte a WiFi
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "Conectado a WiFi.");
    }
    else
    {
        ESP_LOGE(TAG, "Error al conectar.");
    }
}

// Configuración del UART para la pantalla Nextion
void uart_init()
{
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0);
}

// Función para enviar un comando a Nextion
void nextion_send_command(const char *command)
{
    uart_write_bytes(UART_NUM, command, strlen(command)); // Enviar comando
    uint8_t end_cmd[] = {0xFF, 0xFF, 0xFF};               // Terminador Nextion
    uart_write_bytes(UART_NUM, (const char *)end_cmd, 3);
}

// Función para sincronizar el tiempo con NTP
void sync_ntp_time()
{
    ESP_LOGI(TAG, "Inicializando NTP...");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org"); // Servidor NTP
    sntp_init();

    // Configurar zona horaria de República Dominicana (GMT-4)
    setenv("TZ", "EST4", 1);
    tzset();

    // Esperar sincronización
    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (2024 - 1900) && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Esperando sincronización NTP...");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (retry == retry_count)
    {
        ESP_LOGE(TAG, "Fallo al sincronizar NTP.");
        return;
    }

    ESP_LOGI(TAG, "Hora sincronizada correctamente.");
}

// Tarea para imprimir la hora y enviarla a Nextion
void print_time_task(void *pvParameter)
{
    char date_buffer[48]; // Aumentado a 48 bytes
    char time_buffer[48]; // Aumentado a 48 bytes

    while (1)
    {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        // Convertir a formato 12 horas
        int hour = timeinfo.tm_hour;
        char *am_pm = (hour >= 12) ? "PM" : "AM";
        if (hour > 12)
        {
            hour -= 12;
        }
        else if (hour == 0)
        {
            hour = 12;
        }

        // Formatear fecha (YYYY-MM-DD)
        snprintf(date_buffer, sizeof(date_buffer), "t0.txt=\"%02d-%02d-%04d\"",
                 timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);

        // Formatear hora (HH:MM:SS AM/PM)
        snprintf(time_buffer, sizeof(time_buffer), "t1.txt=\"%02d:%02d:%02d %s\"",
                 hour, timeinfo.tm_min, timeinfo.tm_sec, am_pm);

        // Enviar los comandos a la pantalla Nextion
        nextion_send_command(date_buffer);
        nextion_send_command(time_buffer);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Actualiza cada segundo
    }
}

void app_main()
{
    wifi_init();     // Conectar a WiFi
    uart_init();     // Inicializar UART para Nextion
    sync_ntp_time(); // Sincronizar NTP
    nextion_send_command("t0.txt=\"Test Nextion\"");

    // Crear tarea para imprimir y enviar datos a Nextion
    xTaskCreate(print_time_task, "print_time_task", 4096, NULL, 5, NULL);
}
