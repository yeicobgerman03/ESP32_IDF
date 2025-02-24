#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/timers.h"
#include "driver/adc.h"

#define led_pin 2
int interval_time = 100;
int timer_Id = 1;
uint8_t led_state = 0;
static const char *tag = "main";
TimerHandle_t xTimers;

esp_err_t init_led(void);
esp_err_t Blink_led(void);
esp_err_t init_timer(void);

void vTimerCallback(TimerHandle_t pxTimer)
{
    ESP_LOGI(tag, "the timer was called");
    Blink_led();
}

void app_main(void)
{
    init_led();
    init_timer();
}

esp_err_t init_led(void)
{

    gpio_reset_pin(led_pin);
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
    return ESP_OK;
}
esp_err_t Blink_led(void)
{

    led_state = !led_state;
    gpio_set_level(led_pin, led_state);
    return ESP_OK;
}

esp_err_t init_timer(void)
{
    xTimers = xTimerCreate("Timer",                        // Just a text name, not used by the kernel.
                           (pdMS_TO_TICKS(interval_time)), // The timer period in ticks.
                           pdTRUE,                         // The timers will auto-reload themselves when they expire.
                           (void *)timer_Id,               // Assign each timer a unique id equal to its array index.
                           vTimerCallback                  // Each timer calls the same callback when it expires.
    );

    if (xTimers == NULL)
    {
        // The timer was not created.
    }
    else
    {
        // Start the timer.  No block time is specified, and even if one was
        // it would be ignored because the scheduler has not yet been
        // started.
        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            // The timer could not be set into the Active state.
        }
    }
    return ESP_OK;
}
