#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define led_pin 2
uint8_t LED_state;

esp_err_t init_led(void);
esp_err_t Blink_led(void);
void app_main(void)
{
    init_led();

    for (;;)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);
        Blink_led();
        printf("led state:%u", LED_state);
    }
}

esp_err_t init_led(void)
{
    gpio_reset_pin(led_pin);
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
    return ESP_OK;
}
esp_err_t Blink_led(void)
{
    LED_state = !LED_state;
    gpio_set_level(led_pin, LED_state);
    return ESP_OK;
}
