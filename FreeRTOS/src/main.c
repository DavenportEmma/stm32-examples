#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_gcc.h"
#include "stm32f722xx.h"

/*
TODO

simple keyboard diode matrix on breadboard to test input polling
    then move on to using encoder and decoder ICs

rotary encoder test

outputing multiple midi streams

led matrix test
    then move on to using enocder and decoder ICs

midi sync in

using a flash memory IC for storing sequence data

*/

// pd8
#define GPIOD_AFRH      (*(volatile uint32_t *)(GPIOD_BASE + 0x24))

#define SEND_TIMEOUT 10000

void init_uart(void) { 
    RCC->AHB1ENR |= 0x8;            // enable clock for gpiod
    GPIOD->MODER &= ~(0x3 << 16);   // Clear mode bits for pd8
    GPIOD->MODER |= 0x20000;        // set alternate mode for pb6
    GPIOD_AFRH |= 0x7;              // set pd8 to af7

    
    RCC->APB1ENR |= (1 << 18);           // Enable clock for USART3
    RCC->DCKCFGR2 |= (1 << 4);
    RCC->CR |= RCC_CR_HSION;
    // Configure USART6
    USART3->CR1 &= ~(0xFFFFFFFF);   // clear cr1
    USART3->BRR |= 0x683;  // Assuming APB2 clock is 16 MHz
    USART3->CR1 |= 0x1; // enable usart
    USART3->CR1 |= 0x8; // enable tx
}

void init_gpio(void) {
    RCC->AHB1ENR |= 2;
    GPIOB->MODER |= 0x4000;
    GPIOB->ODR |= 0x80;
}

void send(char* msg, int len) {
    for(int i = 0; i < len; i++) {
        uint32_t timeout_counter = 0;

        while(!(USART3->ISR & (1 << 7))) {
            timeout_counter++;
            if(timeout_counter >= SEND_TIMEOUT) {
                return;
            }
        }
        USART3->TDR = msg[i];
    }
}


void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    __disable_irq();
}

void UARTTask() {
    char msg[] = "sent from task\n\r";
    while(1) {
        send(msg, sizeof(msg)-1);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void blinkLEDTask() {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 1000;
    xLastWakeTime = xTaskGetTickCount();

    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        GPIOB->ODR ^= 0x80;
        xLastWakeTime = xTaskGetTickCount();
    }
}

int main(void) {
    init_uart();
    init_gpio();

    char msg[] = "uart initialised\n\r";
    send(msg, sizeof(msg)-1);

    xTaskCreate(UARTTask, "UART Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(blinkLEDTask, "blink led task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1) {

    }
}