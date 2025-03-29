// #include <stdio.h>
// #include "driver/gpio.h"
// #include "driver/i2c.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"

// #define I2C_MASTER_SCL_IO    22  // GPIO para SCL
// #define I2C_MASTER_SDA_IO    21  // GPIO para SDA
// #define I2C_MASTER_NUM       I2C_NUM_0
// #define LCD_ADDR             0x27  // Dirección I2C del LCD
// #define BUTTON_PIN           GPIO_NUM_4  // Botón en GPIO 4

// static QueueHandle_t button_queue;
// volatile bool lcd_on = true; // Estado de la LCD

// // Función para escribir en el LCD
// void lcd_send_command(uint8_t command) {
//     uint8_t data[2] = {command, lcd_on ? 0x08 : 0x00}; // 0x08 = Luz encendida, 0x00 = Luz apagada
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, data, 2, pdMS_TO_TICKS(100));
// }

// // ISR para el botón
// void IRAM_ATTR button_isr_handler(void *arg) {
//     int dummy = 0;
//     xQueueSendFromISR(button_queue, &dummy, NULL);
// }

// // Tarea para manejar el botón con debounce
// void button_task(void *arg) {
//     int dummy;
//     while (1) {
//         if (xQueueReceive(button_queue, &dummy, portMAX_DELAY)) {
//             vTaskDelay(pdMS_TO_TICKS(50));  // Espera 50ms para debounce
            
//             // Verifica si el botón sigue presionado
//             if (gpio_get_level(BUTTON_PIN) == 0) {  
//                 lcd_on = !lcd_on;  // Alternar estado del LCD
//                 lcd_send_command(0x00);  // Enviar nuevo estado al LCD
//                 printf("LCD %s\n", lcd_on ? "ON" : "OFF");

//                 // Espera a que el botón sea soltado para evitar múltiples detecciones
//                 while (gpio_get_level(BUTTON_PIN) == 0) {
//                     vTaskDelay(pdMS_TO_TICKS(10));  // Pequeño delay para no bloquear
//                 }
//             }
//         }
//     }
// }

// void app_main() {
//     // Configurar I2C
//     i2c_config_t i2c_conf = {
//         .mode = I2C_MODE_MASTER,
//         .sda_io_num = I2C_MASTER_SDA_IO,
//         .scl_io_num = I2C_MASTER_SCL_IO,
//         .sda_pullup_en = GPIO_PULLUP_ENABLE,
//         .scl_pullup_en = GPIO_PULLUP_ENABLE,
//         .master.clk_speed = 100000
//     };
//     i2c_param_config(I2C_MASTER_NUM, &i2c_conf);
//     i2c_driver_install(I2C_MASTER_NUM, i2c_conf.mode, 0, 0, 0);

//     // Configurar botón con interrupción
//     gpio_config_t button_config = {
//         .pin_bit_mask = (1ULL << BUTTON_PIN),
//         .mode = GPIO_MODE_INPUT,
//         .pull_up_en = GPIO_PULLUP_ENABLE,
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,
//         .intr_type = GPIO_INTR_NEGEDGE
//     };
//     gpio_config(&button_config);

//     // Crear cola para el botón
//     button_queue = xQueueCreate(10, sizeof(int));
    
//     // Instalar ISR y asignarla al botón
//     gpio_install_isr_service(0);
//     gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

//     // Crear la tarea para manejar el botón con debounce
//     xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

//     // Inicializar LCD encendido
//     lcd_send_command(0x00);
// }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HOLA MUNDO 


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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_MASTER_SDA_IO 21      // Pin SDA
#define I2C_MASTER_SCL_IO 22      // Pin SCL
#define I2C_MASTER_NUM I2C_NUM_0  // I2C port
#define I2C_MASTER_FREQ_HZ 100000 // Frecuencia I2C
#define LCD_ADDR 0x27             // Dirección del LCD (0x27 o 0x3F)

#define LCD_ENABLE 0x04  // Bit de Enable
#define LCD_BACKLIGHT 0x08 // Luz de fondo

// Enviar un byte a la pantalla LCD
void lcd_send_byte(uint8_t data, uint8_t mode) {
    uint8_t high_nibble = (data & 0xF0) | LCD_BACKLIGHT | mode;
    uint8_t low_nibble = ((data << 4) & 0xF0) | LCD_BACKLIGHT | mode;

    uint8_t high_enable = high_nibble | LCD_ENABLE;
    uint8_t low_enable = low_nibble | LCD_ENABLE;

    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_enable, 1, 10 / portTICK_PERIOD_MS);
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);

    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_enable, 1, 10 / portTICK_PERIOD_MS);
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
}

// Inicializar LCD
void lcd_init() {
    vTaskDelay(50 / portTICK_PERIOD_MS); // Esperar estabilización

    lcd_send_byte(0x03, 0);  // Modo 4 bits
    vTaskDelay(5 / portTICK_PERIOD_MS);
    lcd_send_byte(0x03, 0);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    lcd_send_byte(0x03, 0);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    lcd_send_byte(0x02, 0);  // Configurar a 4 bits

    // Configuración de la pantalla
    lcd_send_byte(0x28, 0); // Modo 4 bits, 2 líneas, 5x8 caracteres
    lcd_send_byte(0x0C, 0); // Display ON, cursor OFF
    lcd_send_byte(0x06, 0); // Modo entrada, sin desplazamiento
    lcd_send_byte(0x01, 0); // Limpiar pantalla
    vTaskDelay(50 / portTICK_PERIOD_MS); // Tiempo mayor para limpiar
}

// Mover el cursor a una posición
void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_send_byte(0x80 | (col + offsets[row]), 0);
}

// Escribir un carácter
void lcd_write_char(char c) {
    lcd_send_byte(c, 1);
}

// Escribir una cadena de texto
void lcd_write_string(char *str) {
    while (*str) {
        lcd_write_char(*str++);
    }
}

// Configurar I2C
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

// Programa principal
void app_main() {
    i2c_init();    // Inicializar I2C
    lcd_init();    // Inicializar LCD

    while(1) {
        // Limpiar pantalla antes de mostrar "HOLA MUNDO"
        lcd_send_byte(0x01, 0); // Limpiar pantalla
        vTaskDelay(50 / portTICK_PERIOD_MS); // Esperar más tiempo para limpiar la pantalla

        // Mostrar "HOLA MUNDO" por 2 segundos
        lcd_set_cursor(0, 0);
        lcd_write_string("HOLA MUNDO");
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        // Limpiar pantalla antes de mostrar "HOLA IA"
        lcd_send_byte(0x01, 0); // Limpiar pantalla
        vTaskDelay(50 / portTICK_PERIOD_MS); // Esperar más tiempo para limpiar la pantalla

        // Mostrar "HOLA IA" por 2 segundos
        lcd_set_cursor(0, 0);
        lcd_write_string("HOLA IA");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
