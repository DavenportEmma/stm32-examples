#include <stdint.h>
#include "stm32f722xx.h"

#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))

#define DAC_CR          (*(volatile uint32_t *)(DAC_BASE + 0x00))
#define DAC_DHR12R1     (*(volatile uint32_t *)(DAC_BASE + 0x08))


int main(void) {
    // enable GPIOA
    RCC_AHB1ENR |= 1;

    // set PA4 to analog mode
    GPIOA_MODER |= 0x300;

    // enable DAC
    RCC_APB1ENR |= 0x20000000;

    // enable DAC channel 1
    DAC_CR |= 1;

    // set data format but loading it into DAC_DHR12Rx right aligned 12b
    DAC_DHR12R1 |= 0x800;

    while (1) {
        // Main loop
    }
}
