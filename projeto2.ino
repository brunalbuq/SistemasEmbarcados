#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include <timers.h>

/* Defina as constantes para o LCD */
#define LCD_16X2_I2C_ADDRESS 0x27
#define LCD_16X2_COLS 16
#define LCD_16X2_ROWS 2

/* Defina os pinos que serão utilizados para ligação ao display */
/*LiquidCrystal_I2C lcd(LCD_16X2_I2C_ADDRESS, LCD_16X2_COLS, LCD_16X2_ROWS);*/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/* Defina os pinos para o LED */
#define LED_PIN 1

/* Defina os valores máximos para a leitura ADC */
#define ADC_MAX 1023.0
#define MAX_VOLTAGE_ADC 5.0

/* Filas (queues) */
QueueHandle_t xQueue_LCD;
QueueHandle_t xQueue_LED;

/* Semaforos utilizados */
SemaphoreHandle_t xSerial_semaphore;

/* Contador para controlar o LED */
int ledCounter = 0;

/* Protótipos das funções das tarefas */
void task_sensor(void *pvParameters);
void task_lcd(void *pvParameters);
void task_led(void *pvParameters);

void setup()
{
    /* Inicializa o LCD, liga o backlight e limpa o LCD */
    lcd.begin(16, 2);
    //lcd.init();
    //lcd.backlight();
    lcd.clear();

    /* Inicializa e configura GPIO do LED */
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.begin(9600);

    while (!Serial)
    {
        ; /* Somente vai em frente quando a serial estiver pronta para funcionar */
    }

    /* Criação das filas (queues) */
    xQueue_LCD = xQueueCreate(1, sizeof(float));
    xQueue_LED = xQueueCreate(1, sizeof(float));

    /* Criação dos semaforos */
    xSerial_semaphore = xSemaphoreCreateMutex();

    if (xSerial_semaphore == NULL)
    {
        Serial.println("Erro: nao e possivel criar o semaforo");
        while (1)
            ; /* Sem semaforo o funcionamento esta comprometido. Nada mais deve ser feito. */
    }

    /* Criação das tarefas */
    xTaskCreate(task_sensor, "sensor", 128, NULL, 3, NULL);
    xTaskCreate(task_lcd, "LCD", 156, NULL, 2, NULL);
    xTaskCreate(task_led, "LED", 128, NULL, 1, NULL);

    /* A partir deste momento, o scheduler de tarefas entra em ação e as tarefas executam */
}

void loop()
{
    /* Tudo é executado nas tarefas. Não há nada a ser feito aqui. */
}

/* --------------------------------------------------*/
/* ---------------------- Tarefas -------------------*/
/* --------------------------------------------------*/

void task_sensor(void *pvParameters)
{
    (void)pvParameters;
    int adc_read = 0;
    UBaseType_t uxHighWaterMark;
    float voltage = 0.0;

    while (1)
    {
        adc_read = analogRead(0);
        voltage = ((float)adc_read / ADC_MAX) * MAX_VOLTAGE_ADC;

        /* Envia tensão lida em A0 para as tarefas a partir de filas */
        xQueueOverwrite(xQueue_LCD, (void *)&voltage);
        xQueueOverwrite(xQueue_LED, (void *)&voltage);

        /* Espera um segundo */
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        /* Para fins de teste de ocupação de stack, printa na serial o high water mark */
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.print("task_sensor high water mark (words): ");
        Serial.println(uxHighWaterMark);
        Serial.println("---");
        xSemaphoreGive(xSerial_semaphore);
    }
}

void task_lcd(void *pvParameters)
{
    (void)pvParameters;
    float voltage_rcv = 0.0;
    UBaseType_t uxHighWaterMark;

    while (1)
    {
        /* Espera até algo ser recebido na queue */
        xQueueReceive(xQueue_LCD, (void *)&voltage_rcv, portMAX_DELAY);

        /* Uma vez recebida a informação na queue, a escreve no display LCD */
        lcd.setCursor(0, 0);
        lcd.print("Tensao ADC:");
        lcd.setCursor(0, 1);
        lcd.print("                "); // Limpar a linha
        lcd.setCursor(0, 1);
        lcd.print(voltage_rcv);
        lcd.setCursor(4, 1);
        lcd.print("V");

        /* Exibe o relógio no LCD */
        lcd.setCursor(11, 0);
        lcd.print(millis() / 1000); // Exibe o tempo em segundos

        /* Para fins de teste de ocupação de stack, printa na serial o high water mark */
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.print("task_lcd high water mark (words): ");
        Serial.println(uxHighWaterMark);
        Serial.println("---");
        xSemaphoreGive(xSerial_semaphore);
    }
}

void task_led(void *pvParameters)
{
    (void)pvParameters;
    float voltage_rcv = 0.0;
    UBaseType_t uxHighWaterMark;

    while (1)
    {
        if (ledCounter < 4)
        {
            digitalWrite(LED_PIN, HIGH);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            digitalWrite(LED_PIN, LOW);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            ledCounter++;
        }
        else
        {
            /* Desliga o LED por 5 segundos */
            digitalWrite(LED_PIN, LOW);
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            ledCounter = 0; // Reinicia o contador após 5 segundos de desligamento
        }

        /* Para fins de teste de ocupação de stack, printa na serial o high water mark */
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        Serial.print("task_led high water mark (words): ");
        Serial.println(uxHighWaterMark);
        Serial.println("---");
        xSemaphoreGive(xSerial_semaphore);
    }
}
