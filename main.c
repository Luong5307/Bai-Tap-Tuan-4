#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

// C?u trúc tham s? LED
typedef struct {
    uint16_t pin_mask;
    float frequency;
} LedTaskParam_t;

// Khai báo 3 c?u hình LED t?i PA0, PA1, PA2
static LedTaskParam_t led1_config = { (1 << 0), 0.1f };   // PA0 - 0.1 Hz
static LedTaskParam_t led2_config = { (1 << 1), 1.0f };   // PA1 - 1.0 Hz
static LedTaskParam_t led3_config = { (1 << 2), 10.0f };  // PA2 - 10.0 Hz

// Kh?i t?o GPIOA (PA0, PA1, PA2) b?ng thanh ghi CMSIS
void GPIO_Config(void) {
    // B?t clock cho PORTA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // C?u hình PA0, PA1, PA2 là Output Push-Pull 50MHz
    GPIOA->CRL &= ~(0x00000FFF);
    GPIOA->CRL |=  (0x00000333);
}

// Task di?u khi?n nh?p nháy LED
void LedControlTask(void *pvParameters) {
    LedTaskParam_t *ledParam = (LedTaskParam_t *)pvParameters;
    uint32_t half_period_ms = (uint32_t)(500.0f / ledParam->frequency);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        // Ð?o tr?ng thái chân LED
        GPIOA->ODR ^= ledParam->pin_mask;

        // Tr? bán chu k?
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(half_period_ms));
    }
}

int main(void) {
    // Kh?i t?o ph?n c?ng
    GPIO_Config();

    // T?o 3 Task di?u khi?n 3 LED
    xTaskCreate(LedControlTask, "LED_0.1Hz", 128, (void*)&led1_config, 1, NULL);
    xTaskCreate(LedControlTask, "LED_1Hz",   128, (void*)&led2_config, 1, NULL);
    xTaskCreate(LedControlTask, "LED_10Hz",  128, (void*)&led3_config, 1, NULL);

    // B?t d?u ch?y b? l?p l?ch FreeRTOS
    vTaskStartScheduler();

    while (1) {
    }
}