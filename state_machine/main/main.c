#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>
#include "esp_now.h"

#define ESTADO_ESPERANDO 0
#define ESTADO_PROCESANDO 1
#define ESTADO_VERIFICANDO_ENVASE 2
#define ESTADO_DISPENSANDO 3
#define ESTADO_ERROR 4

#define PASTILLA_NO_SELECCIONADA -1

#define TIEMPO_DISPENSADO_MS 2000 // Tiempo que el servo estará activado

static int estado_actual = ESTADO_ESPERANDO;
static int pastilla_seleccionada = PASTILLA_NO_SELECCIONADA;
TimerHandle_t dispensador_timer;

void callback_timer(TimerHandle_t xTimer)
{
    // Cuando el timer expira, apagamos el servo
    ESP_LOGI("DISPENSADOR", "Tiempo de dispensado completado, apagando servo");
    estado_actual = ESTADO_ESPERANDO;
}

void estado_esperando()
{
    ESP_LOGI("DISPENSADOR", "Esperando datos MQTT...");
    // Aquí se debe esperar un mensaje de MQTT y procesarlo
    // Supongamos que llega un mensaje en JSON con el medicamento seleccionado
}

void estado_procesando(const char *mensaje_json)
{
    ESP_LOGI("DISPENSADOR", "Procesando solicitud...");

    cJSON *root = cJSON_Parse(mensaje_json);
    if (!root)
    {
        ESP_LOGE("DISPENSADOR", "Error al parsear JSON");
        estado_actual = ESTADO_ERROR;
        return;
    }

    cJSON *medicamento = cJSON_GetObjectItem(root, "medicamento");
    if (medicamento)
    {
        if (strcmp(medicamento->valuestring, "A") == 0)
            pastilla_seleccionada = 0;
        else if (strcmp(medicamento->valuestring, "B") == 0)
            pastilla_seleccionada = 1;
        else if (strcmp(medicamento->valuestring, "C") == 0)
            pastilla_seleccionada = 2;
        else
            pastilla_seleccionada = PASTILLA_NO_SELECCIONADA;
    }

    cJSON_Delete(root);

    if (pastilla_seleccionada == PASTILLA_NO_SELECCIONADA)
    {
        ESP_LOGE("DISPENSADOR", "Medicamento no válido");
        estado_actual = ESTADO_ERROR;
    }
    else
    {
        estado_actual = ESTADO_VERIFICANDO_ENVASE;
    }
}

void estado_verificando_envase()
{
    ESP_LOGI("DISPENSADOR", "Verificando si el envase está presente...");
    // Aquí se lee el sensor ultrasónico. Si detecta el envase, pasamos a DISPENSANDO.
    // Supongamos que la función verificar_envase() retorna true si el envase está presente.

    bool envase_presente = true; // Aquí iría la lectura real

    if (envase_presente)
    {
        estado_actual = ESTADO_DISPENSANDO;
    }
    else
    {
        ESP_LOGE("DISPENSADOR", "Error: Envase no presente");
        estado_actual = ESTADO_ERROR;
    }
}

void estado_dispensando()
{
    ESP_LOGI("DISPENSADOR", "Dispensando pastilla %d...", pastilla_seleccionada);

    // Activar el servo correspondiente
    // Supongamos que una función activar_servo(n) lo enciende
    // activar_servo(pastilla_seleccionada);

    // Iniciar un temporizador para apagar el servo después de cierto tiempo
    xTimerStart(dispensador_timer, 0);

    estado_actual = ESTADO_ESPERANDO; // Volvemos a esperar una nueva solicitud
}

void estado_error()
{
    ESP_LOGE("DISPENSADOR", "Entrando en estado de error...");
    // Aquí podrías agregar un LED de error o una notificación MQTT
    estado_actual = ESTADO_ESPERANDO; // Después del error, vuelve a esperar
}

void app_main()
{
    dispensador_timer = xTimerCreate("DispensadorTimer", pdMS_TO_TICKS(TIEMPO_DISPENSADO_MS), pdFALSE, 0, callback_timer);

    while (1)
    {
        switch (estado_actual)
        {
        case ESTADO_ESPERANDO:
            estado_esperando();
            break;
        case ESTADO_PROCESANDO:
            // Aquí pasaríamos el mensaje recibido por MQTT
            estado_procesando("{\"medicamento\":\"A\"}");
            break;
        case ESTADO_VERIFICANDO_ENVASE:
            estado_verificando_envase();
            break;
        case ESTADO_DISPENSANDO:
            estado_dispensando();
            break;
        case ESTADO_ERROR:
            estado_error();
            break;
        default:
            ESP_LOGE("DISPENSADOR", "Estado no reconocido");
            estado_actual = ESTADO_ERROR;
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Pequeño delay para evitar alto consumo de CPU
    }
}
