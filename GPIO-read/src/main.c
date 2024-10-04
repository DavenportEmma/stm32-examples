#include "stm32f722xx.h"

// user button is at pc13

int main(void) {
    RCC->AHB1LPENR |= (1 << 2);     // enable gpioc clock
    GPIOC->MODER &= ~(3 << 26);

    RCC->AHB1ENR |= 2;
    GPIOB->MODER |= 0x4000;

    RCC->APB2ENR |= (1 << 14);      // enable clock for syscfg

    SYSCFG->EXTICR[3] |= (2 << 4);  // enable external interrupt for pc13

    EXTI->IMR |= (1 << 13);         // enable interrupts

    EXTI->FTSR |= (1 << 13);        // falling edge trigger

    NVIC_EnableIRQ(EXTI15_10_IRQn);

    while(1) {

    }
}

void EXTI15_10_IRQHandler(void) {
    if(EXTI->PR & (1 << 13)) {
        EXTI->PR |= (1 << 13);      // clear interrupt flag
            GPIOB->ODR ^= 0x80;
    }
}

