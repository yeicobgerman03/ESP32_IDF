#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define led_pin_R 32
#define led_pin_G 25
#define led_pin_B 26
#define STACK_SIZE 2048
#define tskIDLE_PRIORITY 1
#define time_led_R 200
#define time_led_G 500
#define time_led_B 100

static const char *tag = "main";

esp_err_t init_led(void);
esp_err_t create_Task(void);
void vTaskCodeR(void *pvParameters);
void vTaskCodeG(void *pvParameters);
void vTaskCodeB(void *pvParameters);

void app_main()
{
    create_Task();
    init_led();
}

esp_err_t init_led(void)
{
    gpio_reset_pin(led_pin_R);
    gpio_set_direction(led_pin_R, GPIO_MODE_OUTPUT);

    gpio_reset_pin(led_pin_G);
    gpio_set_direction(led_pin_G, GPIO_MODE_OUTPUT);

    gpio_reset_pin(led_pin_B);
    gpio_set_direction(led_pin_B, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t create_Task(void)
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    return ESP_OK;
    xTaskCreate(vTaskCodeR, "NAME",
                STACK_SIZE,
                &ucParameterToPass,
                tskIDLE_PRIORITY,
                &xHandle);
    configASSERT(xHandle);

    return ESP_OK;
}

void vTaskCodeR(void *pvParameters)
{
    for (;;)
    {
        ESP_LOGI(tag, "The led R is executing");
        gpio_set_level(led_pin_R, 1);
        vTaskDelay(pdMS_TO_TICKS(time_led_R));
        gpio_set_level(led_pin_R, 0);
        vTaskDelay(pdMS_TO_TICKS(time_led_R));
    }
    ESP_LOGE(tag, "the led R was not executed");
}

void vTaskCodeG(void *pvParameters)
{
    for (;;)
    {
        ESP_LOGI(tag, "The led G is executing");
        gpio_set_level(led_pin_G, 1);
        vTaskDelay(pdMS_TO_TICKS(time_led_G));
        gpio_set_level(led_pin_G, 0);
        vTaskDelay(pdMS_TO_TICKS(time_led_G));
    }
    ESP_LOGE(tag, "the led G was not executed");
}

void vTaskCodeB(void *pvParameters)
{
    for (;;)
    {
        ESP_LOGI(tag, "The led B is executing");
        gpio_set_level(led_pin_B, 1);
        vTaskDelay(pdMS_TO_TICKS(time_led_B));
        gpio_set_level(led_pin_B, 0);
        vTaskDelay(pdMS_TO_TICKS(time_led_B));
    }
    ESP_LOGE(tag, "the led B was not executed");
}
