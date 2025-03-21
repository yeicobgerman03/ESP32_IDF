#include <stdio.h>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_2   // LED en GPIO2
#define UART_NUM UART_NUM_2    // UART2 para Nextion

#define TX_PIN GPIO_NUM_17
#define RX_PIN GPIO_NUM_16

static const char *TAG = "NEXTION_LED";

void nextion_init() {
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0);
}

void app_main() {
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    nextion_init();

    uint8_t data[10];  // Buffer más grande por si llegan más datos
    while (1) {
        int len = uart_read_bytes(UART_NUM, data, sizeof(data), 100 / portTICK_PERIOD_MS);
        if (len > 0) {
            printf("Datos recibidos: ");
            for (int i = 0; i < len; i++) {
                printf("%02X ", data[i]);  // Imprime cada byte en hexadecimal
            }
            printf("\n");

            ESP_LOG_BUFFER_HEX(TAG, data, len); // Muestra el buffer completo en logs

            // Recorre los datos recibidos y actúa según el valor
            for (int i = 0; i < len; i++) {
                if (data[i] == 0x31) { // '1' en ASCII (0x31) -> Enciende LED
                    gpio_set_level(LED_GPIO, 1);
                    ESP_LOGI(TAG, "LED ENCENDIDO");
                } else if (data[i] == 0x30) { // '0' en ASCII (0x30) -> Apaga LED
                    gpio_set_level(LED_GPIO, 0);
                    ESP_LOGI(TAG, "LED APAGADO");
                }
            }
        }
    }
}
