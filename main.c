#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct {
    uint16_t pin_mask;
    float frequency;
} LedTaskParam_t;

static LedTaskParam_t led1_config = { (1 << 0), 0.1f };   // PA0 - 0.1 Hz
static LedTaskParam_t led2_config = { (1 << 1), 1.0f };   // PA1 - 1.0 Hz
static LedTaskParam_t led3_config = { (1 << 2), 10.0f };  // PA2 - 10.0 Hz

void GPIO_Config(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    GPIOA->CRL &= ~(0x00000FFF);
    GPIOA->CRL |=  (0x00000333);
}

void LedControlTask(void *pvParameters) {
    LedTaskParam_t *ledParam = (LedTaskParam_t *)pvParameters;
    uint32_t half_period_ms = (uint32_t)(500.0f / ledParam->frequency);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        GPIOA->ODR ^= ledParam->pin_mask;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(half_period_ms));
    }
}

int main(void) {
    GPIO_Config();
    xTaskCreate(LedControlTask, "LED_0.1Hz", 128, (void*)&led1_config, 1, NULL);
    xTaskCreate(LedControlTask, "LED_1Hz",   128, (void*)&led2_config, 1, NULL);
    xTaskCreate(LedControlTask, "LED_10Hz",  128, (void*)&led3_config, 1, NULL);
    vTaskStartScheduler();
    while (1) {
    }
}
