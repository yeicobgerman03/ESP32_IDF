// #include "driver/gpio.h"
// #include "driver/ledc.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include "esp_timer.h"

// #define CLK_PIN  18  // Pin CLK del encoder
// #define DT_PIN   19  // Pin DT del encoder
// #define LED_PIN  32  // GPIO de salida PWM

// volatile int encoder_position = 0;  // Posición del encoder
// #define MIN_POSITION 0    // Límite inferior (0% duty cycle)
// #define MAX_POSITION 100  // Límite superior (100% duty cycle)
// #define DEBOUNCE_TIME_US 1000  // **2ms** de debounce para mejor respuesta

// volatile int64_t last_interrupt_time = 0;  // Último tiempo de interrupción

// void IRAM_ATTR encoder_isr_handler(void *arg) {
//     int64_t current_time = esp_timer_get_time();  // Tiempo actual en microsegundos

//     if ((current_time - last_interrupt_time) < DEBOUNCE_TIME_US) {
//         return;  // Ignorar rebotes
//     }
//     last_interrupt_time = current_time;  // Actualizar tiempo

//     int clk_state = gpio_get_level(CLK_PIN);
//     int dt_state  = gpio_get_level(DT_PIN);

//     if (dt_state != clk_state) {
//         if (encoder_position < MAX_POSITION) {
//             encoder_position++;  // Giro horario
//         }
//     } else {
//         if (encoder_position > MIN_POSITION) {
//             encoder_position--;  // Giro antihorario
//         }
//     }
// }

// // Función para actualizar el PWM
// void update_pwm(int duty) {
//     int duty_scaled = (duty * 8191) / 100;  // Convertir 0-100% a 0-8191 (resolución 13 bits)
//     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty_scaled);
//     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
// }

// void app_main() {
//     // Configuración de CLK con interrupción
//     gpio_config_t io_conf = {
//         .pin_bit_mask = (1ULL << CLK_PIN),
//         .mode = GPIO_MODE_INPUT,
//         .pull_up_en = GPIO_PULLUP_ENABLE,
//         .intr_type = GPIO_INTR_NEGEDGE  // Detectar flanco de bajada
//     };
//     gpio_config(&io_conf);

//     // Configuración de DT como entrada normal
//     io_conf.pin_bit_mask = (1ULL << DT_PIN);
//     gpio_config(&io_conf);

//     // Instalar ISR
//     gpio_install_isr_service(0);
//     gpio_isr_handler_add(CLK_PIN, encoder_isr_handler, NULL);

//     // Configurar PWM en GPIO 32
//     ledc_timer_config_t ledc_timer = {
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .duty_resolution = LEDC_TIMER_13_BIT,
//         .timer_num = LEDC_TIMER_0,
//         .freq_hz = 1000,  // Frecuencia 1kHz
//         .clk_cfg = LEDC_AUTO_CLK
//     };
//     ledc_timer_config(&ledc_timer);

//     ledc_channel_config_t ledc_channel = {
//         .gpio_num = LED_PIN,
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .channel = LEDC_CHANNEL_0,
//         .timer_sel = LEDC_TIMER_0,
//         .duty = 0,  // Iniciar en 0%
//         .hpoint = 0
//     };
//     ledc_channel_config(&ledc_channel);

//     int last_position = -1;  // Para evitar actualizaciones innecesarias

//     while (1) {
//         if (encoder_position != last_position) {  // Solo actualizar si cambia
//             printf("Encoder: %d%%\n", encoder_position);
//             update_pwm(encoder_position);
//             last_position = encoder_position;
//         }
//         vTaskDelay(10 / portTICK_PERIOD_MS);  // **Menor retardo** (10ms) para más rapidez
//     }
// }



// #include <stdio.h>
// #include "driver/i2c.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// #define I2C_MASTER_SDA_IO 21      // Pin SDA
// #define I2C_MASTER_SCL_IO 22      // Pin SCL
// #define I2C_MASTER_NUM I2C_NUM_0  // I2C port
// #define I2C_MASTER_FREQ_HZ 100000 // Frecuencia I2C
// #define LCD_ADDR 0x27             // Dirección del LCD (0x27 o 0x3F)

// #define LCD_ENABLE 0x04  // Bit de Enable
// #define LCD_BACKLIGHT 0x08 // Luz de fondo

// // Enviar un byte a la pantalla LCD
// void lcd_send_byte(uint8_t data, uint8_t mode) {
//     uint8_t high_nibble = (data & 0xF0) | LCD_BACKLIGHT | mode;
//     uint8_t low_nibble = ((data << 4) & 0xF0) | LCD_BACKLIGHT | mode;

//     uint8_t high_enable = high_nibble | LCD_ENABLE;
//     uint8_t low_enable = low_nibble | LCD_ENABLE;

//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_enable, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);

//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_enable, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
// }


// // Inicializar LCD
// void lcd_init() {
//     vTaskDelay(50 / portTICK_PERIOD_MS); // Esperar estabilización

//     lcd_send_byte(0x03, 0);  // Modo 4 bits
//     vTaskDelay(5 / portTICK_PERIOD_MS);
//     lcd_send_byte(0x03, 0);
//     vTaskDelay(5 / portTICK_PERIOD_MS);
//     lcd_send_byte(0x03, 0);
//     vTaskDelay(1 / portTICK_PERIOD_MS);
//     lcd_send_byte(0x02, 0);  // Configurar a 4 bits

//     // Configuración de la pantalla
//     lcd_send_byte(0x28, 0); // Modo 4 bits, 2 líneas, 5x8 caracteres
//     lcd_send_byte(0x0C, 0); // Display ON, cursor OFF
//     lcd_send_byte(0x06, 0); // Modo entrada, sin desplazamiento
//     lcd_send_byte(0x01, 0); // Limpiar pantalla
//     vTaskDelay(2 / portTICK_PERIOD_MS);
// }

// // Mover el cursor a una posición
// void lcd_set_cursor(uint8_t row, uint8_t col) {
//     uint8_t offsets[] = {0x00, 0x40, 0x14, 0x54};
//     lcd_send_byte(0x80 | (col + offsets[row]), 0);
// }

// // Escribir un carácter
// void lcd_write_char(char c) {
//     lcd_send_byte(c, 1);
// }

// // Escribir una cadena de texto
// void lcd_write_string(char *str) {
//     while (*str) {
//         lcd_write_char(*str++);
//     }
// }

// // Configurar I2C
// void i2c_init() {
//     i2c_config_t conf = {
//         .mode = I2C_MODE_MASTER,
//         .sda_io_num = I2C_MASTER_SDA_IO,
//         .scl_io_num = I2C_MASTER_SCL_IO,
//         .sda_pullup_en = GPIO_PULLUP_ENABLE,
//         .scl_pullup_en = GPIO_PULLUP_ENABLE,
//         .master.clk_speed = I2C_MASTER_FREQ_HZ
//     };
//     i2c_param_config(I2C_MASTER_NUM, &conf);
//     i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
// }

// // Programa principal
// void app_main() {
//     i2c_init();    // Inicializar I2C
//     lcd_init();    // Inicializar LCD
//     lcd_set_cursor(0, 0);
//     lcd_write_string("HOLA MUNDO"); // Escribir mensaje en LCD
// }


#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

// Definición de pines
#define CLK_PIN  18  // Pin CLK del encoder
#define DT_PIN   19  // Pin DT del encoder
#define LED_PIN  32  // GPIO de salida PWM
#define I2C_MASTER_SDA_IO 21      // Pin SDA
#define I2C_MASTER_SCL_IO 22      // Pin SCL
#define I2C_MASTER_NUM I2C_NUM_0  // I2C port
#define I2C_MASTER_FREQ_HZ 100000 // Frecuencia I2C
#define LCD_ADDR 0x27             // Dirección del LCD (0x27 o 0x3F)

// Variables globales
volatile int encoder_position = 0;  // Posición del encoder
#define MIN_POSITION 0    // Límite inferior (0% duty cycle)
#define MAX_POSITION 100  // Límite superior (100% duty cycle)
#define DEBOUNCE_TIME_US 5000  // **5ms** de debounce para evitar múltiples interrupciones
volatile int64_t last_interrupt_time = 0;  // Último tiempo de interrupción

// Función para actualizar el PWM
void update_pwm(int duty) {
    int duty_scaled = (duty * 8191) / 100;  // Convertir 0-100% a 0-8191 (resolución 13 bits)
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty_scaled);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
}

// Función de interrupción del encoder
void IRAM_ATTR encoder_isr_handler(void *arg) {
    int64_t current_time = esp_timer_get_time();  // Tiempo actual en microsegundos

    if ((current_time - last_interrupt_time) < DEBOUNCE_TIME_US) {
        return;  // Ignorar rebotes
    }
    last_interrupt_time = current_time;  // Actualizar tiempo

    int clk_state = gpio_get_level(CLK_PIN);
    int dt_state  = gpio_get_level(DT_PIN);

    if (dt_state != clk_state) {
        if (encoder_position < MAX_POSITION) {
            encoder_position++;  // Giro horario
        }
    } else {
        if (encoder_position > MIN_POSITION) {
            encoder_position--;  // Giro antihorario
        }
    }
}

// Función para inicializar el LCD
void lcd_send_byte(uint8_t data, uint8_t mode) {
    uint8_t high_nibble = (data & 0xF0) | 0x08 | mode;
    uint8_t low_nibble = ((data << 4) & 0xF0) | 0x08 | mode;

    uint8_t high_enable = high_nibble | 0x04;
    uint8_t low_enable = low_nibble | 0x04;

    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_enable, 1, 10 / portTICK_PERIOD_MS);
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);

    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_enable, 1, 10 / portTICK_PERIOD_MS);
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
}

// Inicialización del LCD
void lcd_init() {
    vTaskDelay(50 / portTICK_PERIOD_MS); // Esperar estabilización
    lcd_send_byte(0x03, 0);  // Modo 4 bits
    vTaskDelay(5 / portTICK_PERIOD_MS);
    lcd_send_byte(0x03, 0);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    lcd_send_byte(0x03, 0);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    lcd_send_byte(0x02, 0);  // Configurar a 4 bits
    lcd_send_byte(0x28, 0);  // Modo 4 bits, 2 líneas, 5x8 caracteres
    lcd_send_byte(0x0C, 0);  // Display ON, cursor OFF
    lcd_send_byte(0x06, 0);  // Modo entrada, sin desplazamiento
    lcd_send_byte(0x01, 0);  // Limpiar pantalla
    vTaskDelay(2 / portTICK_PERIOD_MS);
}

// Configuración de I2C
void i2c_init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
}

// Función para mover el cursor a una posición en el LCD
void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_send_byte(0x80 | (col + offsets[row]), 0);
}

// Función para escribir una cadena en el LCD
void lcd_write_string(char *str) {
    while (*str) {
        lcd_send_byte(*str++, 1);
    }
}

// Configuración de los pines y PWM
void setup_pwm() {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << CLK_PIN) | (1ULL << DT_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&io_conf);

    // Instalar ISR para el encoder
    gpio_install_isr_service(0);
    gpio_isr_handler_add(CLK_PIN, encoder_isr_handler, NULL);

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,  // Frecuencia 1kHz
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,  // Iniciar en 0%
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
}

// Programa principal
void app_main() {
    i2c_init();    // Inicializar I2C
    lcd_init();    // Inicializar LCD
    lcd_set_cursor(0, 0);
    lcd_write_string("Velocidad: "); // Escribir mensaje inicial en LCD

    setup_pwm();   // Configurar PWM y encoder

    int last_position = -1;  // Para evitar actualizaciones innecesarias

    while (1) {
        if (encoder_position != last_position) {  // Solo actualizar si cambia
            char duty_text[16];
            snprintf(duty_text, sizeof(duty_text), "%d%%", encoder_position); // Crear el texto del duty cycle
            lcd_set_cursor(1, 0);
            lcd_write_string(duty_text); // Mostrar el duty cycle en LCD
            update_pwm(encoder_position);
            last_position = encoder_position;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);  // Menor retardo (10ms) para más rapidez
    }
}
