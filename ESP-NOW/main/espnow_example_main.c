// #include <stdio.h>
// #include "string.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_now.h"
// #include "esp_wifi.h"
// #include "esp_netif.h"
// #include "esp_mac.h"
// #include "esp_event.h"
// #include "nvs_flash.h"
// #include "esp_log.h"
// #include "driver/gpio.h"

// #define ESP_CHANNEL 1
// #define LED 2

// static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
// static const char *TAG = "esp_now_init";

// static esp_err_t init_wifi(void)
// {
//     wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

//     esp_netif_init();
//     esp_event_loop_create_default();
//     nvs_flash_init();
//     esp_wifi_init(&wifi_init_config);
//     esp_wifi_set_mode(WIFI_MODE_STA);
//     esp_wifi_set_storage(WIFI_STORAGE_FLASH);
//     esp_wifi_start();
//     ESP_LOGI(TAG, "WiFi init completed");
//     return ESP_OK;
// }

// void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len)
// {
//     ESP_LOGI(TAG, "Data received from " MACSTR ": %s", MAC2STR(esp_now_info->src_addr), data);
// }

// void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//     if (status == ESP_NOW_SEND_SUCCESS)
//     {
//         ESP_LOGI(TAG, "ESP_NOW_SEND_SUCCESS");
//     }
//     else
//     {
//         ESP_LOGW(TAG, "ESP_NOW_SEND_FAIL");
//     }
// }

// static esp_err_t init_esp_now(void)
// {
//     esp_err_t ret = esp_now_init();
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "ESP-NOW no pudo inicializarse, error: %d", ret);
//         return ret;
//     }

//     esp_now_register_recv_cb(recv_cb);
//     esp_now_register_send_cb(send_cb);
//     ESP_LOGI(TAG, "ESP-NOW inicializado correctamente");
//     return ESP_OK;
// }

// static esp_err_t register_peer(uint8_t *peer_addr)
// {
//     esp_now_peer_info_t esp_now_peer_info = {};
//     memcpy(esp_now_peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
//     esp_now_peer_info.channel = ESP_CHANNEL;
//     esp_now_peer_info.ifidx = WIFI_IF_STA;

//     esp_err_t result = esp_now_add_peer(&esp_now_peer_info);
//     if (result == ESP_OK) {
//         ESP_LOGI(TAG, "Peer agregado correctamente");
//     } else {
//         ESP_LOGE(TAG, "Error al agregar peer: %d", result);
//     }
//     return result;
// }

// static esp_err_t esp_now_send_data(const uint8_t *peer_addr, const char *data)
// {
//     return esp_now_send(peer_addr, (uint8_t *)data, strlen(data) + 1);
// }

// static void send_task(void *pvParameter)
// {
//     while (1)
//     {
//         const char *msg = "Hello ESP-NOW";
//         esp_err_t result = esp_now_send(peer_mac, (uint8_t *)msg, strlen(msg) + 1);

//         if (result == ESP_OK) {
//             ESP_LOGI(TAG, "Mensaje enviado: %s", msg);
//         } else {
//             ESP_LOGE(TAG, "Error al enviar mensaje: %d", result);
//         }

//         vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo
//     }
// }

// void app_main(void)
// {
//     ESP_ERROR_CHECK(init_wifi());
//     ESP_ERROR_CHECK(init_esp_now());
//     ESP_ERROR_CHECK(register_peer(peer_mac));

//     xTaskCreate(&send_task, "send_task", 2048, NULL, 5, NULL);
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CON LED//
// #include <stdio.h>
// #include "string.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_now.h"
// #include "esp_wifi.h"
// #include "esp_netif.h"
// #include "esp_mac.h"
// #include "esp_event.h"
// #include "nvs_flash.h"
// #include "esp_log.h"
// #include "driver/gpio.h"

// #define ESP_CHANNEL 1
// #define LED 2

// static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0x3c, 0x8a, 0x1f, 0xa8, 0x8b, 0x7c};
// static const char *TAG = "ESP_NOW_Emisor";

// static esp_err_t init_wifi(void)
// {
//     wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
//     esp_netif_init();
//     esp_event_loop_create_default();
//     nvs_flash_init();
//     esp_wifi_init(&wifi_init_config);
//     esp_wifi_set_mode(WIFI_MODE_STA);
//     esp_wifi_set_storage(WIFI_STORAGE_FLASH);
//     esp_wifi_start();
//     ESP_LOGI(TAG, "WiFi inicializado");
//     return ESP_OK;
// }

// void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//     ESP_LOGI(TAG, "Estado de envío: %s", status == ESP_NOW_SEND_SUCCESS ? "Éxito" : "Fallo");
// }

// static esp_err_t init_esp_now(void)
// {
//     esp_err_t ret = esp_now_init();
//     if (ret != ESP_OK)
//     {
//         ESP_LOGE(TAG, "ESP-NOW no pudo inicializarse, error: %d", ret);
//         return ret;
//     }

//     esp_now_register_send_cb(send_cb);
//     ESP_LOGI(TAG, "ESP-NOW inicializado");
//     return ESP_OK;
// }

// static esp_err_t register_peer(uint8_t *peer_addr)
// {
//     esp_now_peer_info_t peer_info = {};
//     memcpy(peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
//     peer_info.channel = ESP_CHANNEL;
//     peer_info.ifidx = WIFI_IF_STA;

//     esp_err_t result = esp_now_add_peer(&peer_info);
//     if (result == ESP_OK)
//     {
//         ESP_LOGI(TAG, "Peer agregado correctamente");
//     }
//     else
//     {
//         ESP_LOGE(TAG, "Error al agregar peer: %d", result);
//     }
//     return result;
// }

// static void blink_led()
// {
//     for (int i = 0; i < 1; i++)
//     {
//         gpio_set_level(LED, 1);
//         vTaskDelay(pdMS_TO_TICKS(500));
//         gpio_set_level(LED, 0);
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }

// static void send_task(void *pvParameter)
// {
//     gpio_reset_pin(LED);
//     gpio_set_direction(LED, GPIO_MODE_OUTPUT);

//     while (1)
//     {
//         const char *msg = "BLINK";
//         esp_now_send(peer_mac, (uint8_t *)msg, strlen(msg) + 1);
//         ESP_LOGI(TAG, "Mensaje enviado: %s", msg);

//         // Parpadear LED localmente
//         blink_led();

//         vTaskDelay(pdMS_TO_TICKS(50)); // Espera 5 segundos antes de enviar de nuevo
//     }
// }

// void app_main(void)
// {
//     ESP_ERROR_CHECK(init_wifi());
//     ESP_ERROR_CHECK(init_esp_now());
//     ESP_ERROR_CHECK(register_peer(peer_mac));

//     xTaskCreate(&send_task, "send_task", 2048, NULL, 5, NULL);
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////

/* ==========================
 * Código del Emisor ESP32
 * ========================== */

//  #include <stdio.h>
//  #include <string.h>
//  #include "freertos/FreeRTOS.h"
//  #include "freertos/task.h"
//  #include "freertos/queue.h"
//  #include "esp_now.h"
//  #include "esp_wifi.h"
//  #include "nvs_flash.h"
//  #include "driver/adc.h"
//  #include "esp_log.h"

//  #define POT_PIN ADC1_CHANNEL_4 // GPIO 32

//  typedef enum {
//      STATE_READ_ADC,
//      STATE_SEND_DATA
//  } state_t;

//  typedef struct {
//      uint8_t duty_cycle;
//  } espnow_data_t;

//  static uint8_t peer_mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // MAC del receptor
//  static state_t current_state = STATE_READ_ADC;

//  void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
//      ESP_LOGI("ESP-NOW", "Send status: %s", status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
//  }

//  void send_task(void *pvParameter) {
//      espnow_data_t data;
//      while (1) {
//          switch (current_state) {
//              case STATE_READ_ADC:
//                  // Leer el ADC y convertir a porcentaje de duty cycle (0-100)
//                  int adc_reading = adc1_get_raw(POT_PIN);
//                  data.duty_cycle = (adc_reading * 100) / 4095;
//                  current_state = STATE_SEND_DATA;
//                  break;

//              case STATE_SEND_DATA:
//                  esp_now_send(peer_mac, (uint8_t *)&data, sizeof(data));
//                  current_state = STATE_READ_ADC;
//                  break;
//          }
//          vTaskDelay(pdMS_TO_TICKS(100));
//      }
//  }

//  void app_main() {
//      nvs_flash_init();
//      wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//      esp_wifi_init(&cfg);
//      esp_wifi_set_mode(WIFI_MODE_STA);
//      esp_wifi_start();

//      esp_now_init();
//      esp_now_register_send_cb(espnow_send_cb);

//      esp_now_peer_info_t peer_info = {};
//      memcpy(peer_info.peer_addr, peer_mac, 6);
//      peer_info.channel = 0;
//      peer_info.encrypt = false;
//      esp_now_add_peer(&peer_info);

//      adc1_config_width(ADC_WIDTH_BIT_12);
//      adc1_config_channel_atten(POT_PIN, ADC_ATTEN_DB_12);

//      xTaskCreate(send_task, "send_task", 2048, NULL, 1, NULL);
//  }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODIGO POT_ESP-NOW_PWM
// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "esp_now.h"
// #include "esp_wifi.h"
// #include "nvs_flash.h"
// #include "driver/adc.h"
// #include "esp_log.h"
// #include "esp_event.h"

// #define ESP_CHANNEL 0
// #define POT_PIN ADC1_CHANNEL_4                                    // GPIO 32
// static uint8_t peer_mac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // MAC del receptor

// typedef enum
// {
//     STATE_READ_ADC,
//     STATE_SEND_DATA
// } state_t;

// typedef struct
// {
//     uint8_t duty_cycle;
// } espnow_data_t;

// static state_t current_state = STATE_READ_ADC;
// static const char *TAG = "ESP_NOW";

// static esp_err_t wifi_init(void)
// {
//     ESP_ERROR_CHECK(nvs_flash_init()); // Inicializar NVS

//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());

//     wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
//     ESP_ERROR_CHECK(esp_wifi_start());

//     ESP_LOGI(TAG, "WiFi inicializado correctamente");
//     return ESP_OK;
// }

// void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//     ESP_LOGI(TAG, "Estado de envío: %s", status == ESP_NOW_SEND_SUCCESS ? "Éxito" : "Fallo");
// }

// static esp_err_t init_esp_now(void)
// {
//     if (esp_now_init() != ESP_OK)
//     {
//         ESP_LOGE(TAG, "Error al inicializar ESP-NOW");
//         return ESP_FAIL;
//     }

//     ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_send_cb));
//     ESP_LOGI(TAG, "ESP-NOW inicializado correctamente");
//     return ESP_OK;
// }

// static esp_err_t register_peer(void)
// {
//     esp_now_peer_info_t peer_info = {0};
//     memcpy(peer_info.peer_addr, peer_mac, 6);
//     peer_info.channel = ESP_CHANNEL;
//     peer_info.encrypt = false;
//     peer_info.ifidx = ESP_IF_WIFI_STA;

//     if (esp_now_add_peer(&peer_info) != ESP_OK)
//     {
//         ESP_LOGE(TAG, "Error al registrar el peer");
//         return ESP_FAIL;
//     }

//     ESP_LOGI(TAG, "Peer registrado correctamente");
//     return ESP_OK;
// }

// void send_task(void *pvParameter)
// {
//     espnow_data_t data;
//     while (1)
//     {
//         switch (current_state)
//         {
//         case STATE_READ_ADC:
//             // Leer el ADC y convertir a porcentaje de duty cycle (0-100)
//             data.duty_cycle = (adc1_get_raw(POT_PIN) * 100) / 4095;
//             current_state = STATE_SEND_DATA;
//             break;

//         case STATE_SEND_DATA:
//             if (esp_now_send(peer_mac, (uint8_t *)&data, sizeof(data)) != ESP_OK)
//             {
//                 ESP_LOGE(TAG, "Error al enviar datos por ESP-NOW");
//             }
//             current_state = STATE_READ_ADC;
//             break;
//         }
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

// void app_main()
// {
//     ESP_ERROR_CHECK(wifi_init());
//     ESP_ERROR_CHECK(init_esp_now());
//     ESP_ERROR_CHECK(register_peer());

//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(POT_PIN, ADC_ATTEN_DB_12);

//     xTaskCreate(send_task, "send_task", 2048, NULL, 1, NULL);
// }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// codigo con encoder_lcd_esp_now

// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "driver/gpio.h"
// #include "driver/ledc.h"
// #include "driver/i2c.h"
// #include "esp_log.h"
// #include "esp_timer.h"
// #include "esp_now.h"
// #include "esp_wifi.h"
// #include "nvs_flash.h"
// #include "esp_event.h"

// // Definición de pines
// #define CLK_PIN  18  // Pin CLK del encoder
// #define DT_PIN   19  // Pin DT del encoder
// #define LED_PIN  32  // GPIO de salida PWM
// #define I2C_MASTER_SDA_IO 21      // Pin SDA LCD
// #define I2C_MASTER_SCL_IO 22      // Pin SCL LCD
// #define I2C_MASTER_NUM I2C_NUM_0  // I2C port
// #define I2C_MASTER_FREQ_HZ 100000 // Frecuencia I2C
// #define LCD_ADDR 0x27             // Dirección del LCD (0x27 o 0x3F)

// // Variables globales
// volatile int encoder_position = 0;
// #define MIN_POSITION 0
// #define MAX_POSITION 100
// #define DEBOUNCE_TIME_US 5000
// volatile int64_t last_interrupt_time = 0;

// // Definición de la MAC del receptor ESP-NOW
// static uint8_t peer_mac[] = {0x3C, 0x8A, 0x1F, 0xA8, 0x8B, 0x7C};

// typedef struct {
//     uint8_t duty_cycle;
// } espnow_data_t;

// static const char *TAG = "ESP_NOW";

// // Función para actualizar el PWM
// void update_pwm(int duty) {
//     int duty_scaled = (duty * 8191) / 100;
//     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty_scaled);
//     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
// }

// // Interrupción del encoder
// void IRAM_ATTR encoder_isr_handler(void *arg) {
//     int64_t current_time = esp_timer_get_time();
//     if ((current_time - last_interrupt_time) < DEBOUNCE_TIME_US) return;
//     last_interrupt_time = current_time;

//     int clk_state = gpio_get_level(CLK_PIN);
//     int dt_state  = gpio_get_level(DT_PIN);

//     if (dt_state != clk_state) {
//         if (encoder_position < MAX_POSITION) encoder_position++;
//     } else {
//         if (encoder_position > MIN_POSITION) encoder_position--;
//     }
// }

// // Inicialización de I2C
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

// // Función para enviar byte a LCD
// void lcd_send_byte(uint8_t data, uint8_t mode) {
//     uint8_t high_nibble = (data & 0xF0) | 0x08 | mode;
//     uint8_t low_nibble = ((data << 4) & 0xF0) | 0x08 | mode;

//     uint8_t high_enable = high_nibble | 0x04;
//     uint8_t low_enable = low_nibble | 0x04;

//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_enable, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);

//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_enable, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
// }

// // Inicialización del LCD
// void lcd_init() {
//     vTaskDelay(50 / portTICK_PERIOD_MS);
//     lcd_send_byte(0x03, 0);
//     lcd_send_byte(0x02, 0);
//     lcd_send_byte(0x28, 0);
//     lcd_send_byte(0x0C, 0);
//     lcd_send_byte(0x06, 0);
//     lcd_send_byte(0x01, 0);
// }

// // Función para mover el cursor
// void lcd_set_cursor(uint8_t row, uint8_t col) {
//     uint8_t offsets[] = {0x00, 0x40};
//     lcd_send_byte(0x80 | (col + offsets[row]), 0);
// }

// // Función para escribir texto en el LCD
// void lcd_write_string(char *str) {
//     while (*str) lcd_send_byte(*str++, 1);
// }

// // Función para imprimir texto largo
// void lcd_print_text(char *str) {
//     int i = 0;
//     while (str[i] != '\0') {
//         if (i == 16) { // Si llega al final de la primera línea
//             lcd_set_cursor(1, 0); // Mover a la segunda línea
//         }
//         lcd_send_byte(str[i], 1); // Enviar el carácter
//         i++;
//     }
// }

// // Configuración de pines y PWM
// void setup_pwm() {
//     gpio_config_t io_conf = {
//         .pin_bit_mask = (1ULL << CLK_PIN) | (1ULL << DT_PIN),
//         .mode = GPIO_MODE_INPUT,
//         .pull_up_en = GPIO_PULLUP_ENABLE,
//         .intr_type = GPIO_INTR_NEGEDGE
//     };
//     gpio_config(&io_conf);

//     gpio_install_isr_service(0);
//     gpio_isr_handler_add(CLK_PIN, encoder_isr_handler, NULL);

//     ledc_timer_config_t ledc_timer = {
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .duty_resolution = LEDC_TIMER_13_BIT,
//         .timer_num = LEDC_TIMER_0,
//         .freq_hz = 1000,
//         .clk_cfg = LEDC_AUTO_CLK
//     };
//     ledc_timer_config(&ledc_timer);

//     ledc_channel_config_t ledc_channel = {
//         .gpio_num = LED_PIN,
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .channel = LEDC_CHANNEL_0,
//         .timer_sel = LEDC_TIMER_0,
//         .duty = 0,
//         .hpoint = 0
//     };
//     ledc_channel_config(&ledc_channel);
// }

// // Configuración de ESP-NOW
// static esp_err_t wifi_init(void) {
//     ESP_ERROR_CHECK(nvs_flash_init());
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());

//     wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
//     ESP_ERROR_CHECK(esp_wifi_start());

//     return ESP_OK;
// }

// void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
//     ESP_LOGI(TAG, "Estado de envío: %s", status == ESP_NOW_SEND_SUCCESS ? "Éxito" : "Fallo");
// }

// static esp_err_t init_esp_now(void) {
//     if (esp_now_init() != ESP_OK) return ESP_FAIL;
//     esp_now_register_send_cb(espnow_send_cb);
//     return ESP_OK;
// }

// static esp_err_t register_peer(void) {
//     esp_now_peer_info_t peer_info = {0};
//     memcpy(peer_info.peer_addr, peer_mac, 6);
//     peer_info.channel = 0;
//     peer_info.encrypt = false;
//     peer_info.ifidx = ESP_IF_WIFI_STA;
//     return esp_now_add_peer(&peer_info);
// }

// // Enviar duty cycle por ESP-NOW
// void send_task(void *pvParameter) {
//     espnow_data_t data;
//     while (1) {
//         data.duty_cycle = encoder_position;
//         esp_now_send(peer_mac, (uint8_t *)&data, sizeof(data));
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

// void app_main() {
//     i2c_init();
//     lcd_init();
//     lcd_set_cursor(0, 0);

//     lcd_write_string("Speed:");

//     setup_pwm();
//     wifi_init();
//     init_esp_now();
//     register_peer();

//     xTaskCreate(send_task, "send_task", 2048, NULL, 1, NULL);

//     while (1) {
//         update_pwm(encoder_position);
//         lcd_set_cursor(1, 0); // Mover a la segunda línea
//         char duty_text[16];
//         snprintf(duty_text, sizeof(duty_text), "%-3d%%", encoder_position);
//         lcd_print_text(duty_text); // Usar lcd_print_text para manejar texto largo
//         vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// PRUEBA

// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "driver/gpio.h"
// #include "driver/ledc.h"
// #include "driver/i2c.h"
// #include "esp_log.h"
// #include "esp_timer.h"
// #include "esp_now.h"
// #include "esp_wifi.h"
// #include "nvs_flash.h"
// #include "esp_event.h"

// // Definición de pines
// #define CLK_PIN 18                // Pin CLK del encoder
// #define DT_PIN 19                 // Pin DT del encoder
// #define LED_PIN 32                // GPIO de salida PWM
// #define I2C_MASTER_SDA_IO 21      // Pin SDA LCD
// #define I2C_MASTER_SCL_IO 22      // Pin SCL LCD
// #define I2C_MASTER_NUM I2C_NUM_0  // I2C port
// #define I2C_MASTER_FREQ_HZ 100000 // Frecuencia I2C
// #define LCD_ADDR 0x27             // Dirección del LCD (0x27 o 0x3F)
// #define ERROR_LED_PIN 2           // Pin GPIO para el LED de error

// // Variables globales
// volatile int encoder_position = 0;
// #define MIN_POSITION 0
// #define MAX_POSITION 100
// #define DEBOUNCE_TIME_US 5000
// volatile int64_t last_interrupt_time = 0;

// // Definición de la MAC del receptor ESP-NOW
// static uint8_t peer_mac[] = {0x3C, 0x8A, 0x1F, 0xA8, 0x8B, 0x7C};

// typedef struct
// {
//     uint8_t duty_cycle;
// } espnow_data_t;

// static const char *TAG = "ESP_NOW";

// // Función para actualizar el PWM
// void update_pwm(int duty)
// {
//     int duty_scaled = (duty * 8191) / 100;
//     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty_scaled);
//     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
// }

// // Interrupción del encoder
// void IRAM_ATTR encoder_isr_handler(void *arg)
// {
//     int64_t current_time = esp_timer_get_time();
//     if ((current_time - last_interrupt_time) < DEBOUNCE_TIME_US)
//         return;
//     last_interrupt_time = current_time;

//     int clk_state = gpio_get_level(CLK_PIN);
//     int dt_state = gpio_get_level(DT_PIN);

//     if (dt_state != clk_state)
//     {
//         if (encoder_position < MAX_POSITION)
//             encoder_position++;
//     }
//     else
//     {
//         if (encoder_position > MIN_POSITION)
//             encoder_position--;
//     }
// }

// // Inicialización de I2C
// void i2c_init()
// {
//     i2c_config_t conf = {
//         .mode = I2C_MODE_MASTER,
//         .sda_io_num = I2C_MASTER_SDA_IO,
//         .scl_io_num = I2C_MASTER_SCL_IO,
//         .sda_pullup_en = GPIO_PULLUP_ENABLE,
//         .scl_pullup_en = GPIO_PULLUP_ENABLE,
//         .master.clk_speed = I2C_MASTER_FREQ_HZ};
//     i2c_param_config(I2C_MASTER_NUM, &conf);
//     i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
// }

// // Función para enviar byte a LCD
// void lcd_send_byte(uint8_t data, uint8_t mode)
// {
//     uint8_t high_nibble = (data & 0xF0) | 0x08 | mode;
//     uint8_t low_nibble = ((data << 4) & 0xF0) | 0x08 | mode;

//     uint8_t high_enable = high_nibble | 0x04;
//     uint8_t low_enable = low_nibble | 0x04;

//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_enable, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &high_nibble, 1, 10 / portTICK_PERIOD_MS);

//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_enable, 1, 10 / portTICK_PERIOD_MS);
//     i2c_master_write_to_device(I2C_MASTER_NUM, LCD_ADDR, &low_nibble, 1, 10 / portTICK_PERIOD_MS);
// }

// // Inicialización del LCD
// void lcd_init()
// {
//     vTaskDelay(50/ portTICK_PERIOD_MS);
//     lcd_send_byte(0x03, 0);
//     lcd_send_byte(0x02, 0);
//     lcd_send_byte(0x28, 0);
//     lcd_send_byte(0x0C, 0);
//     lcd_send_byte(0x06, 0);
//     lcd_send_byte(0x01, 0);
// }

// // Función para mover el cursor
// void lcd_set_cursor(uint8_t row, uint8_t col)
// {
//     uint8_t offsets[] = {0x00, 0x40};
//     lcd_send_byte(0x80 | (col + offsets[row]), 0);
// }

// // Función para escribir texto en el LCD
// void lcd_write_string(char *str)
// {
//     while (*str)
//         lcd_send_byte(*str++, 1);
// }

// // Función para imprimir texto largo
// void lcd_print_text(char *str)
// {
//     int i = 0;
//     while (str[i] != '\0')
//     {
//         if (i == 16)
//         {                         // Si llega al final de la primera línea
//             lcd_set_cursor(1, 0); // Mover a la segunda línea
//         }
//         lcd_send_byte(str[i], 1); // Enviar el carácter
//         i++;
//     }
// }

// // Configuración de pines y PWM
// void setup_pwm()
// {
//     gpio_config_t io_conf = {
//         .pin_bit_mask = (1ULL << CLK_PIN) | (1ULL << DT_PIN),
//         .mode = GPIO_MODE_INPUT,
//         .pull_up_en = GPIO_PULLUP_ENABLE,
//         .intr_type = GPIO_INTR_NEGEDGE};
//     gpio_config(&io_conf);

//     gpio_install_isr_service(0);
//     gpio_isr_handler_add(CLK_PIN, encoder_isr_handler, NULL);

//     ledc_timer_config_t ledc_timer = {
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .duty_resolution = LEDC_TIMER_13_BIT,
//         .timer_num = LEDC_TIMER_0,
//         .freq_hz = 1000,
//         .clk_cfg = LEDC_AUTO_CLK};
//     ledc_timer_config(&ledc_timer);

//     ledc_channel_config_t ledc_channel = {
//         .gpio_num = LED_PIN,
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .channel = LEDC_CHANNEL_0,
//         .timer_sel = LEDC_TIMER_0,
//         .duty = 0,
//         .hpoint = 0};
//     ledc_channel_config(&ledc_channel);
// }

// // Configuración de ESP-NOW
// static esp_err_t wifi_init(void)
// {
//     ESP_ERROR_CHECK(nvs_flash_init());
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());

//     wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
//     ESP_ERROR_CHECK(esp_wifi_start());

//     return ESP_OK;
// }

// void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
//     if (status == ESP_NOW_SEND_SUCCESS) {
//         ESP_LOGI(TAG, "Envío exitoso");
//         gpio_set_level(ERROR_LED_PIN, 0);
//     } else {
//         ESP_LOGE(TAG, "Fallo en el envío");
//         gpio_set_level(ERROR_LED_PIN, 1);
//         lcd_set_cursor(0, 0);
//         lcd_write_string("Error");
//     }
// }

// static esp_err_t init_esp_now(void)
// {
//     if (esp_now_init() != ESP_OK)
//         return ESP_FAIL;
//     esp_now_register_send_cb(espnow_send_cb);
//     return ESP_OK;
// }

// static esp_err_t register_peer(void)
// {
//     esp_now_peer_info_t peer_info = {0};
//     memcpy(peer_info.peer_addr, peer_mac, 6);
//     peer_info.channel = 0;
//     peer_info.encrypt = false;
//     peer_info.ifidx = ESP_IF_WIFI_STA;
//     return esp_now_add_peer(&peer_info);
// }

// // Enviar duty cycle por ESP-NOW
// void send_task(void *pvParameter)
// {
//     espnow_data_t data;
//     while (1)
//     {
//         data.duty_cycle = encoder_position;
//         esp_now_send(peer_mac, (uint8_t *)&data, sizeof(data));
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

// void app_main()
// {

//     gpio_reset_pin(ERROR_LED_PIN);
//     gpio_set_direction(ERROR_LED_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_level(ERROR_LED_PIN, 0); // Asegura que el LED esté apagado inicialmente

//     i2c_init();
//     lcd_init();
//     lcd_set_cursor(0, 0);

//     lcd_write_string("Speed:");

//     setup_pwm();
//     wifi_init();
//     init_esp_now();
//     register_peer();

//     xTaskCreate(send_task, "send_task", 2048, NULL, 1, NULL);

//     while (1)
//     {
//         update_pwm(encoder_position);
//         lcd_set_cursor(1, 0); // Mover a la segunda línea
//         char duty_text[16];
//         snprintf(duty_text, sizeof(duty_text), "%-3d%%", encoder_position);
//         lcd_print_text(duty_text); // Usar lcd_print_text para manejar texto largo
//         vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//PRUEBA -

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
    vTaskDelay(50/ portTICK_PERIOD_MS);
    lcd_send_byte(0x03, 0);
    lcd_send_byte(0x02, 0);
    lcd_send_byte(0x28, 0);
    lcd_send_byte(0x0C, 0);
    lcd_send_byte(0x06, 0);
    lcd_send_byte(0x01, 0);
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

void espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        ESP_LOGI(TAG, "Envío exitoso");
        gpio_set_level(ERROR_LED_PIN, 0);
    } else {
        ESP_LOGE(TAG, "Fallo en el envío");
        gpio_set_level(ERROR_LED_PIN, 1);

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

    lcd_write_string("speed:");

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ejemplo


