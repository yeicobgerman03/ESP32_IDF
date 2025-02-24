#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "esp_log.h"

#define led_pin 2
uint8_t state_led = 0;
TimerHandle_t xTimers;
int interval = 100;
int timerId = 1;

esp_err_t init_led(void);
esp_err_t Blink_led(void);
esp_err_t set_timer(void);
static const char* tag = "Main";

void vTimerCallback(TimerHandle_t pxTimer)
{
    ESP_LOGI(tag,"the timer was executed\n");
    Blink_led();
}
void app_main(void)
{
    init_led();
    set_timer();
}
esp_err_t init_led(void)
{
    gpio_reset_pin(led_pin);
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
    return ESP_OK;
}
esp_err_t Blink_led(void)
{
    state_led = !state_led;
    gpio_set_level(led_pin, state_led);
    return ESP_OK;
}

esp_err_t set_timer(void)
{

    xTimers = xTimerCreate("Timer",                   // Just a text name, not used by the kernel.
                           (pdMS_TO_TICKS(interval)), // The timer period in ticks.
                           pdTRUE,                    // The timers will auto-reload themselves when they expire.
                           (void *)timerId,           // Assign each timer a unique id equal to its array index.
                           vTimerCallback             // Each timer calls the same callback when it expires.
    );

    if (xTimers == NULL)
    {
        // The timer was not created.
        ESP_LOGE(tag,"The timer was not created");
    }
    else
    {
        // Start the timer.  No block time is specified, and even if one was
        // it would be ignored because the scheduler has not yet been
        // started.
        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            // The timer could not be set into the Active state.
            ESP_LOGE(tag,"The timer could not be set into the Active state.");
        }
    }

    return ESP_OK;
}
