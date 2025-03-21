//primer codigo pwm y timer////////////////////////////////////////////////////////////////////////////////////////////
// #include <stdio.h>
// #include "driver/gpio.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/timers.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include "driver/ledc.h"

// #define led_pin 2
// uint8_t state_led = 0;
// TimerHandle_t xTimers;
// int interval = 200;
// int timerId = 1;
// int duty_R = 400;
// int duty_G = 100;
// int duty_B = 0;

// esp_err_t init_led(void);
// esp_err_t Blink_led(void);
// esp_err_t set_timer(void);
// esp_err_t set_pwm(void);
// esp_err_t set_pwm_duty(void);

// static const char *tag = "Main";

// void vTimerCallback(TimerHandle_t pxTimer)
// {
//     ESP_LOGI(tag, "the timer was executed\n");

//     duty_R += 10;
//     if (duty_R > 1023)
//         duty_R = 0;

//     duty_G += 10;
//     if (duty_G > 1023)
//         duty_G = 0;

//     duty_B += 10;
//     if (duty_B > 1023)
//         duty_B = 0;

//     set_pwm_duty();
//     Blink_led();

// }
// void app_main(void)
// {
//     init_led();
//     set_pwm();
//     set_pwm_duty();
//     set_timer();
// }
// esp_err_t init_led(void)
// {
//     gpio_reset_pin(led_pin);
//     gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
//     return ESP_OK;
// }
// esp_err_t Blink_led(void)
// {
//     state_led = !state_led;
//     gpio_set_level(led_pin, state_led);
//     return ESP_OK;
// }

// esp_err_t set_timer(void)

// {

//     xTimers = xTimerCreate("Timer",                   // Just a text name, not used by the kernel.
//                            (pdMS_TO_TICKS(interval)), // The timer period in ticks.
//                            pdTRUE,                    // The timers will auto-reload themselves when they expire.
//                            (void *)timerId,           // Assign each timer a unique id equal to its array index.
//                            vTimerCallback             // Each timer calls the same callback when it expires.
//     );

//     if (xTimers == NULL)
//     {
//         // The timer was not created.
//         ESP_LOGE(tag, "The timer was not created");
//     }
//     else
//     {
//         // Start the timer.  No block time is specified, and even if one was
//         // it would be ignored because the scheduler has not yet been
//         // started.
//         if (xTimerStart(xTimers, 0) != pdPASS)
//         {
//             // The timer could not be set into the Active state.
//             ESP_LOGE(tag, "The timer could not be set into the Active state.");
//         }
//     }

//     return ESP_OK;
// }

// esp_err_t set_pwm(void)
// {

//     ledc_channel_config_t ledc_channel_config_R = {0};
//     ledc_channel_config_R.gpio_num = 33;
//     ledc_channel_config_R.speed_mode = LEDC_HIGH_SPEED_MODE;
//     ledc_channel_config_R.channel = LEDC_CHANNEL_0;
//     ledc_channel_config_R.intr_type = LEDC_INTR_DISABLE;
//     ledc_channel_config_R.timer_sel = LEDC_TIMER_0;
//     ledc_channel_config_R.duty = 0;

//     ledc_channel_config_t ledc_channel_config_G = {0};
//     ledc_channel_config_G.gpio_num = 25;
//     ledc_channel_config_G.speed_mode = LEDC_HIGH_SPEED_MODE;
//     ledc_channel_config_G.channel = LEDC_CHANNEL_1;
//     ledc_channel_config_G.intr_type = LEDC_INTR_DISABLE;
//     ledc_channel_config_G.timer_sel = LEDC_TIMER_0;
//     ledc_channel_config_G.duty = 0;

//     ledc_channel_config_t ledc_channel_config_B = {0};
//     ledc_channel_config_B.gpio_num = 26;
//     ledc_channel_config_B.speed_mode = LEDC_HIGH_SPEED_MODE;
//     ledc_channel_config_B.channel = LEDC_CHANNEL_2;
//     ledc_channel_config_B.intr_type = LEDC_INTR_DISABLE;
//     ledc_channel_config_B.timer_sel = LEDC_TIMER_0;
//     ledc_channel_config_B.duty = 0;

//     ledc_channel_config(&ledc_channel_config_R);
//     ledc_channel_config(&ledc_channel_config_G);
//     ledc_channel_config(&ledc_channel_config_B);

//     ledc_timer_config_t timer_config = {0};
//     timer_config.speed_mode = LEDC_HIGH_SPEED_MODE;
//     timer_config.duty_resolution = LEDC_TIMER_10_BIT;
//     timer_config.timer_num = LEDC_TIMER_0;
//     timer_config.freq_hz = 20000; // 20kHZ

//     ledc_timer_config(&timer_config);

//     return ESP_OK;
// }

// esp_err_t set_pwm_duty(void)
// {
//     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty_R);
//     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, duty_G);
//     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, duty_B);

//     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
//     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
//     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);

//     return ESP_OK;
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//pwm preciso


// #include <stdio.h>
// #include "driver/gpio.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/adc.h"
// #include "esp_log.h"
// #include "driver/ledc.h"

// #define POT_PIN ADC1_CHANNEL_4  // GPIO 32 (ADC1_CHANNEL_4)
// #define LED_PIN 33              // GPIO donde está el LED

// static const char *TAG = "Main";

// void read_potentiometer_task(void *pvParameter);
// esp_err_t init_pwm(void);
// esp_err_t set_pwm_duty(uint32_t duty);

// void app_main(void) {
//     ESP_LOGI(TAG, "Inicio del programa");

//     // Inicializar PWM
//     init_pwm();

//     // Crear tarea para leer el potenciómetro y actualizar el PWM
//     xTaskCreate(read_potentiometer_task, "Read Potentiometer", 2048, NULL, 5, NULL);
// }

// void read_potentiometer_task(void *pvParameter) {
//     int raw_value;
//     uint32_t duty;

//     // Configurar ADC
//     adc1_config_width(ADC_WIDTH_BIT_12);                     // Resolución de 12 bits (0-4095)
//     adc1_config_channel_atten(POT_PIN, ADC_ATTEN_DB_11);     // Atenuación para rango completo (0-3.3V)

//     while (1) {
//         raw_value = adc1_get_raw(POT_PIN);  // Leer el ADC (valor entre 0-4095)
//         duty = raw_value * 1023 / 4095;     // Escalar a rango 0-1023

        

//         set_pwm_duty(duty);  // Actualizar PWM
//         vTaskDelay(pdMS_TO_TICKS(100));  // Esperar 100 ms
//     }
// }

// esp_err_t init_pwm(void) {
//     ledc_timer_config_t timer_config = {
//         .speed_mode       = LEDC_HIGH_SPEED_MODE,
//         .duty_resolution  = LEDC_TIMER_10_BIT,
//         .timer_num        = LEDC_TIMER_0,
//         .freq_hz          = 1000,  // 1kHz
//         .clk_cfg          = LEDC_AUTO_CLK
//     };
//     ledc_timer_config(&timer_config);

//     ledc_channel_config_t ledc_channel = {
//         .gpio_num   = LED_PIN,
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .channel    = LEDC_CHANNEL_0,
//         .intr_type  = LEDC_INTR_DISABLE,
//         .timer_sel  = LEDC_TIMER_0,
//         .duty       = 0
//     };
//     ledc_channel_config(&ledc_channel);

//     return ESP_OK;
// }

// esp_err_t set_pwm_duty(uint32_t duty) {
//     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
//     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
//     return ESP_OK;
// }

//pwm de 10 / 10

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "driver/ledc.h"

#define POT_PIN ADC1_CHANNEL_4  // GPIO 32 (ADC1_CHANNEL_4)
#define LED_PIN 33              // GPIO donde está el LED

static const char *TAG = "Main";

typedef enum {
    STATE_READ_ADC,
    STATE_CALCULATE_DUTY,
    STATE_UPDATE_PWM
} pwm_state_t;

void pwm_state_machine_task(void *pvParameter);
esp_err_t init_pwm(void);
esp_err_t set_pwm_duty(uint32_t duty);

void app_main(void) {
    ESP_LOGI(TAG, "Inicio del programa");
    init_pwm();
    xTaskCreate(pwm_state_machine_task, "PWM State Machine", 2048, NULL, 5, NULL);
}

void pwm_state_machine_task(void *pvParameter) {
    pwm_state_t state = STATE_READ_ADC;
    int raw_value = 0;
    int percentage = 0;
    uint32_t duty = 0;

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(POT_PIN, ADC_ATTEN_DB_11);

    while (1) {
        switch (state) {
            case STATE_READ_ADC:
                raw_value = adc1_get_raw(POT_PIN);
                state = STATE_CALCULATE_DUTY;
                break;
            
            case STATE_CALCULATE_DUTY:
                percentage = (raw_value * 100) / 4095;
                percentage = ((percentage + 5) / 10) * 10; // Redondear al múltiplo de 10 más cercano
                if (percentage > 100) percentage = 100; // Limitar al 100%
                duty = (percentage * 1023) / 100; // Mapear a 0-1023
                state = STATE_UPDATE_PWM;
                break;
            
            case STATE_UPDATE_PWM:
            ESP_LOGI(TAG, "ADC: %d, Porcentaje: %d%%, Duty Cycle: %lu/1023", raw_value, percentage, (unsigned long)duty);
                set_pwm_duty(duty);
                state = STATE_READ_ADC;
                vTaskDelay(pdMS_TO_TICKS(100));
                break;
        }
    }
}

esp_err_t init_pwm(void) {
    ledc_timer_config_t timer_config = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 1000,  // 1kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_config);

    ledc_channel_config_t ledc_channel = {
        .gpio_num   = LED_PIN,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel    = LEDC_CHANNEL_0,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0
    };
    ledc_channel_config(&ledc_channel);

    return ESP_OK;
}

esp_err_t set_pwm_duty(uint32_t duty) {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    return ESP_OK;
}
