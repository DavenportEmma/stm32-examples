#include "stm32f722xx.h"

/*
DIN     PD7
CLK     PD6
CLR     PD5 active low
*/

#define CLR GPIO_ODR_OD5
#define CLK GPIO_ODR_OD6
#define DIN GPIO_ODR_OD7
#define DIN_OFFSET GPIO_ODR_OD7_Pos

void setup() {
    // initialise gpio
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= 0x5400;
    GPIOD->ODR |= 0x0000;
    // pull CLR high
    GPIOD->ODR |= CLR;
}

void loadBit(int b) {
    GPIOD->ODR |=  ((b << DIN_OFFSET) & DIN);
    GPIOD->ODR |= CLK;
    GPIOD->ODR &= ~(0x1 << DIN_OFFSET);
    GPIOD->ODR &= ~CLK;
}

int main(void) {
    setup();

    loadBit(1);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);
    loadBit(0);

    while(1) {

    }
}