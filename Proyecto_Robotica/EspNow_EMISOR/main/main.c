#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "driver/i2c_master.h"

// Definición de pines
#define CLK_PIN 18                // Pin CLK del encoder
#define DT_PIN 19                 // Pin DT del encoder
#define LED_PIN 32                // GPIO de salida PWM
#define I2C_MASTER_SDA_IO 21      // Pin SDA LCD
#define I2C_MASTER_SCL_IO 22      // Pin SCL LCD
#define I2C_MASTER_NUM I2C_NUM_0  // I2C port
#define I2C_MASTER_FREQ_HZ 100000 // Frecuencia I2C
#define LCD_ADDR 0x27             // Dirección del LCD (0x27 o 0x3F)
#define ERROR_LED_PIN 2           // Pin GPIO para el LED de error

// Variables globales
volatile int encoder_position = 0;
#define MIN_POSITION 0
#define MAX_POSITION 100
#define DEBOUNCE_TIME_US 5000
volatile int64_t last_interrupt_time = 0;

// Definición de la MAC del receptor ESP-NOW
static uint8_t peer_mac[] = {0x3C, 0x8A, 0x1F, 0xA8, 0x8B, 0x7C};

typedef struct
{
    uint8_t duty_cycle;
} espnow_data_t;

static const char *TAG = "ESP_NOW";

// Función para actualizar el PWM
void update_pwm(int duty)
{
    int duty_scaled = (duty * 8191) / 100;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty_scaled);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
}

// Interrupción del encoder
void IRAM_ATTR encoder_isr_handler(void *arg)
{
    int64_t current_time = esp_timer_get_time();
    if ((current_time - last_interrupt_time) < DEBOUNCE_TIME_US)
        return;
    last_interrupt_time = current_time;

    int clk_state = gpio_get_level(CLK_PIN);
    int dt_state = gpio_get_level(DT_PIN);

    if (dt_state != clk_state)
    {
        if (encoder_position < MAX_POSITION)
            encoder_position++;
    }
    else
    {
        if (encoder_position > MIN_POSITION)
            encoder_position--;
    }
}

// Inicialización de I2C
void i2c_init()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ};
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
}

// Función para enviar byte a LCD
void lcd_send_byte(uint8_t data, uint8_t mode)
{
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
void lcd_init()
{
    vTaskDelay(50 / portTICK_PERIOD_MS); // Esperar estabilización

    lcd_send_byte(0x03, 0); // Modo 4 bits
    vTaskDelay(5 / portTICK_PERIOD_MS);
    lcd_send_byte(0x03, 0);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    lcd_send_byte(0x03, 0);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    lcd_send_byte(0x02, 0); // Configurar a 4 bits

    // Configuración de la pantalla
    lcd_send_byte(0x28, 0);              // Modo 4 bits, 2 líneas, 5x8 caracteres
    lcd_send_byte(0x0C, 0);              // Display ON, cursor OFF
    lcd_send_byte(0x06, 0);              // Modo entrada, sin desplazamiento
    lcd_send_byte(0x01, 0);              // Limpiar pantalla
    vTaskDelay(50 / portTICK_PERIOD_MS); // Tiempo mayor para limpiar
}

// Función para mover el cursor
void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t offsets[] = {0x00, 0x40};
    lcd_send_byte(0x80 | (col + offsets[row]), 0);
}

// Función para escribir texto en el LCD
void lcd_write_string(char *str)
{
    while (*str)
        lcd_send_byte(*str++, 1);
}

// Función para imprimir texto largo
void lcd_print_text(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (i == 16)
        {                         // Si llega al final de la primera línea
            lcd_set_cursor(1, 0); // Mover a la segunda línea
        }
        lcd_send_byte(str[i], 1); // Enviar el carácter
        i++;
    }
}

// Configuración de pines y PWM
void setup_pwm()
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << CLK_PIN) | (1ULL << DT_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE};
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(CLK_PIN, encoder_isr_handler, NULL);

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel);
}

// Configuración de ESP-NOW
static esp_err_t wifi_init(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

// Función de callback de ESP-NOW para manejar el estado de envío
// Función de callback de ESP-NOW para manejar el estado de envío
void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (status == ESP_NOW_SEND_SUCCESS)
    {
        ESP_LOGI(TAG, "Envío exitoso");
        gpio_set_level(ERROR_LED_PIN, 0); // Apagar LED de error
    }
    else
    {
        ESP_LOGE(TAG, "Fallo en el envío");
        gpio_set_level(ERROR_LED_PIN, 1); // Encender LED de error

        // Reiniciar PWM a 0 en caso de fallo en el envío
        encoder_position = 0;
        update_pwm(encoder_position);
    }
}

static esp_err_t init_esp_now(void)
{
    if (esp_now_init() != ESP_OK)
        return ESP_FAIL;
    esp_now_register_send_cb(espnow_send_cb);
    return ESP_OK;
}

static esp_err_t register_peer(void)
{
    esp_now_peer_info_t peer_info = {0};
    memcpy(peer_info.peer_addr, peer_mac, 6);
    peer_info.channel = 0;
    peer_info.encrypt = false;
    peer_info.ifidx = ESP_IF_WIFI_STA;
    return esp_now_add_peer(&peer_info);
}

// Enviar duty cycle por ESP-NOW
void send_task(void *pvParameter)
{
    espnow_data_t data;
    while (1)
    {
        data.duty_cycle = encoder_position;
        esp_now_send(peer_mac, (uint8_t *)&data, sizeof(data));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main()
{

    gpio_reset_pin(ERROR_LED_PIN);
    gpio_set_direction(ERROR_LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(ERROR_LED_PIN, 0); // Asegura que el LED esté apagado inicialmente

    i2c_init();
    lcd_init();

    lcd_set_cursor(0, 0);
    lcd_send_byte(0x01, 0);                // Limpiar pantalla
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Esperar más tiempo para limpiar la pantalla
    lcd_write_string("INICIANDO...");
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    lcd_set_cursor(0, 0);
    lcd_send_byte(0x01, 0);                // Limpiar pantalla
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Esperar más tiempo para limpiar la pantalla
    lcd_write_string("VELOCIDAD:");

    setup_pwm();
    wifi_init();
    init_esp_now();
    register_peer();

    xTaskCreate(send_task, "send_task", 2048, NULL, 1, NULL);

    while (1)
    {

        update_pwm(encoder_position);
        lcd_set_cursor(1, 0); // Mover a la segunda línea
        char duty_text[16];
        snprintf(duty_text, sizeof(duty_text), "%-3d%%", encoder_position);
        lcd_print_text(duty_text); // Usar lcd_print_text para manejar texto largo
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    
}