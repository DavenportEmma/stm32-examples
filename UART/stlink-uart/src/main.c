#include "stm32f722xx.h"
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

int main(void) {
    init_uart();
    
    char msg[] = "hello\n\r";
    send(msg, sizeof(msg)-1);
    
    while(1) {
    }
}