#include "stm32f722xx.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tasks.h"
#include "keyboard.h"
#include "uart.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    __disable_irq();
}

void setup() {
    // shift register
    // initialise gpio
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= 0x5400;
    GPIOD->ODR |= 0x0000;
    // pull CLR high
    GPIOD->ODR |= CLR;

    // column input pins
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    // port C defaults to input on all pins
    // enable pull down resistors
    GPIOC->PUPDR |= (2 << (ROW_INPUT_0_PIN * 2)) | (2 << (ROW_INPUT_1_PIN * 2)) |
                    (2 << (ROW_INPUT_2_PIN * 2)) | (2 << (ROW_INPUT_3_PIN * 2)) |
                    (2 << (ROW_INPUT_4_PIN * 2)) | (2 << (ROW_INPUT_5_PIN * 2)) |
                    (2 << (ROW_INPUT_6_PIN * 2)) | (2 << (ROW_INPUT_7_PIN * 2));
    
    // initialise stlink uart
    USART_Handler* u;
    u->uart = USART3;
    u->baud = 9600;
    u->gpio = GPIOD;
    u->tx_pin = 8;
    u->rx_pin = 9;
    u->afr_reg = 1;
    u->rx_interrupts = 0;
    int flag = init_uart(u);
}

int main(void) {
    setup();

    xTaskCreate(key_scan_task, "key scan task", 2048, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1) {
        
    }
}