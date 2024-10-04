#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
#include "cmsis_gcc.h"
#include "stm32f722xx.h"
#include "uart.h"
#include "config.h"
#include "menu.h"
#include "tasks.h"

TaskHandle_t xTaskHandle_USART = NULL;
volatile int usart_rx_data = 0;

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    char msg[] = "stack overflow\n\r";
    send_uart(STLINK_UART, msg, sizeof(msg)-1);
    __disable_irq();
}

int main(void) {
    USART_Handler* u_3;
    u_3->uart = USART3;
    u_3->baud = 9600;
    u_3->gpio = GPIOD;
    u_3->rx_pin = 9;
    u_3->tx_pin = 8;
    u_3->afr_reg = 1;
    u_3->rx_interrupts = 1;
    init_uart(u_3);

    NVIC_EnableIRQ(USART3_IRQn);
    USART3->RQR |= USART_RQR_RXFRQ;

    USART_Handler* u_1;
    u_1->uart = USART1;
    u_1->baud = 31250;
    u_1->gpio = GPIOB;
    u_1->rx_pin = 7;
    u_1->tx_pin = 6;
    u_1->afr_reg = 0;
    u_1->rx_interrupts = 0;
    init_uart(u_1);

    xTaskCreate(menu_task, "menu task", configMINIMAL_STACK_SIZE, NULL, 1, &xTaskHandle_USART);
    vTaskStartScheduler();

    while(1) {}
}

void USART3_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (USART3->ISR & USART_ISR_RXNE) {  // Check if RXNE flag is set
        usart_rx_data = USART3->RDR - '0';
        vTaskNotifyGiveFromISR(xTaskHandle_USART, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}