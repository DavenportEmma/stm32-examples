#include "stm32f722xx.h"

#define GPIOB_AFRL      (*(volatile uint32_t *)(GPIOB_BASE + 0x20))

void init_uart(void) { 
    RCC->AHB1ENR |= 0x2;            // enable clock for gpiob
    GPIOB->MODER &= ~(0x3 << 12);   // Clear mode bits for PB6
    GPIOB->MODER |= 0x2000;         // set alternate mode for pb6
    GPIOB_AFRL |= 0x7000000;        // set pc6 to af7

    
    RCC->APB2ENR |= 0x10;           // Enable clock for USART1
    RCC->DCKCFGR2 |= 0x1;
    RCC->CR |= RCC_CR_HSION;
    // Configure USART6
    USART1->CR1 &= ~(0xFFFFFFFF);   // clear cr1
    USART1->BRR |= 0x683;  // Assuming APB2 clock is 16 MHz
    USART1->CR1 |= 0x1; // enable usart
    USART1->CR1 |= 0x8; // enable tx
}

int main(void) {
    init_uart();
    
    while(1) {
        while(!(USART1->ISR & (1 << 7)));
        USART1->TDR = 0xB5;
    }
}