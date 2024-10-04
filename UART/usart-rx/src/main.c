#include "stm32f722xx.h"

#define SEND_TIMEOUT 10000

int main(void) {
       // IO port B clock enable
    RCC->AHB1ENR |= 2;

    // Set GPIOB pin 0 (user LED 1) to output
    GPIOB->MODER |= 0x4000;

    // Turn the LED on
    GPIOB->ODR |= 0x80;

    // PD8 usart3 tx
    // pd9 usart3 rx
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN_Msk;
    GPIOD->AFR[1] |= 0x7 | (0x7 << 4);
    RCC->APB1ENR |= (1 << 18);
    
    GPIOD->MODER &= ~(0x3 << (8 * 2) | 0x3 << (9 * 2));    // Clear mode bits for gpio
    GPIOD->MODER |= (2 << (8 * 2)) | (2 << (9 * 2));         // set alternate mode for pin

    RCC->DCKCFGR2 |= (1 << ((3 - 1) * 2));
    RCC->CR |= RCC_CR_HSION;

    USART3->CR1 &= ~(0xFFFFFFFF);   // clear cr1
    USART3->BRR |= 16000000 / 9600;  // Assuming APB2 clock is 16 MHz
    

    USART3->CR1 |= (1 << 5); // enable rx interrupts
    USART3->CR1 |= 0xC; // enable rx
    USART3->CR1 |= 0x1; // enable usart
    
    NVIC_EnableIRQ(USART3_IRQn);
    while(1) {}
}

void USART3_IRQHandler(void) {
    if (USART3->ISR & USART_ISR_RXNE) {  // Check if RXNE flag is set
        uint8_t data = USART3->RDR;  // Read received data (clears RXNE flag)

        // Optionally, clear other relevant flags by writing to ISR or using specific register operations.
        // USART3->ICR = USART_ICR_ORECF; // Example: Clear Overrun Error Flag if needed

        // Process the received data, e.g., echo it back
        while (!(USART3->ISR & USART_ISR_TXE));  // Wait for TX buffer to be empty
        USART3->TDR = data;  // Echo back the received data
    }

    // Clear any flags you’ve handled or don't need further.
    USART3->RQR |= USART_RQR_RXFRQ;  // Example: Request a flush of the RX buffer (clears RXNE flag)
}