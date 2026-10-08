#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct {
    uint16_t pin;
    float frequency;
} LedConfig_t;

static LedConfig_t led1 = { (1 << 0), 0.1f };   
static LedConfig_t led2 = { (1 << 1), 1.0f };  
static LedConfig_t led3 = { (1 << 2), 10.0f }; 

void GPIO_Config(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
    GPIOA->CRL &= ~(0x00000FFF);      
    GPIOA->CRL |=  (0x00000333);        
}

void LedTask(void *pvParameters) {
    LedConfig_t *config = (LedConfig_t *)pvParameters;
    uint32_t delay_ms = (uint32_t)(500.0f / config->frequency);
    for (;;) {
        GPIOA->ODR ^= config->pin;           
        vTaskDelay(pdMS_TO_TICKS(delay_ms)); 
    }
}

int main(void) {
    GPIO_Config();
    xTaskCreate(LedTask, "LED_0.1Hz", 128, (void*)&led1, 1, NULL);
    xTaskCreate(LedTask, "LED_1Hz",   128, (void*)&led2, 1, NULL);
    xTaskCreate(LedTask, "LED_10Hz",  128, (void*)&led3, 1, NULL);
    vTaskStartScheduler();
    while (1);
}
