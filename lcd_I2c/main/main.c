#include <stdio.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define I2C_MASTER_SCL_IO    22  // GPIO para SCL
#define I2C_MASTER_SDA_IO    21  // GPIO para SDA
#define I2C_MASTER_NUM       I2C_NUM_0
#define LCD_ADDR             0x27  // Dirección I2C del LCD
#define BUTTON_PIN           GPIO_NUM_4  // Botón en GPIO 4

static QueueHandle_t button_queue;
volatile bool lcd_on = true; // Estado de la LCD

// Función para escribir en el LCD
void lcd_send_command(uint8_t command) {
    uint8_t data[2] = {command, lcd_on ? 0x08 : 0x00}; // 0x08 = Luz encendida, 0x00 = Luz apagada
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, data, 2, pdMS_TO_TICKS(100));
}

// ISR para el botón
void IRAM_ATTR button_isr_handler(void *arg) {
    int dummy = 0;
    xQueueSendFromISR(button_queue, &dummy, NULL);
}

// Tarea para manejar el botón con debounce
void button_task(void *arg) {
    int dummy;
    while (1) {
        if (xQueueReceive(button_queue, &dummy, portMAX_DELAY)) {
            vTaskDelay(pdMS_TO_TICKS(50));  // Espera 50ms para debounce
            
            // Verifica si el botón sigue presionado
            if (gpio_get_level(BUTTON_PIN) == 0) {  
                lcd_on = !lcd_on;  // Alternar estado del LCD
                lcd_send_command(0x00);  // Enviar nuevo estado al LCD
                printf("LCD %s\n", lcd_on ? "ON" : "OFF");

                // Espera a que el botón sea soltado para evitar múltiples detecciones
                while (gpio_get_level(BUTTON_PIN) == 0) {
                    vTaskDelay(pdMS_TO_TICKS(10));  // Pequeño delay para no bloquear
                }
            }
        }
    }
}

void app_main() {
    // Configurar I2C
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };
    i2c_param_config(I2C_MASTER_NUM, &i2c_conf);
    i2c_driver_install(I2C_MASTER_NUM, i2c_conf.mode, 0, 0, 0);

    // Configurar botón con interrupción
    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&button_config);

    // Crear cola para el botón
    button_queue = xQueueCreate(10, sizeof(int));
    
    // Instalar ISR y asignarla al botón
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    // Crear la tarea para manejar el botón con debounce
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    // Inicializar LCD encendido
    lcd_send_command(0x00);
}
