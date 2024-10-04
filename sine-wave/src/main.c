// #include <stdint.h>
#include <math.h>
#include "stm32f722xx.h"

#define PI M_PI

#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define TIM2EN          (1 << 0)

#define TIM2_BASE       0x40000000
#define TIM2_CR1        (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_DIER       (*(volatile uint32_t *)(TIM2_BASE + 0x0C))
#define TIM2_SR         (*(volatile uint32_t *)(TIM2_BASE + 0x10))
#define TIM2_EGR        (*(volatile uint32_t *)(TIM2_BASE + 0x14))
#define TIM2_CCMR1      (*(volatile uint32_t *)(TIM2_BASE + 0x18))
#define TIM2_CCER       (*(volatile uint32_t *)(TIM2_BASE + 0x20))
#define TIM2_CNT        (*(volatile uint32_t *)(TIM2_BASE + 0x24))
#define TIM2_PSC        (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR        (*(volatile uint32_t *)(TIM2_BASE + 0x2C))
#define TIM2_CCR1       (*(volatile uint32_t *)(TIM2_BASE + 0x34))

#define NVIC_ISER0      (*(volatile uint32_t *)0xE000E100)
#define TIM2_IRQn       28

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))

#define DAC_CR          (*(volatile uint32_t *)(DAC_BASE + 0x00))
#define DAC_DHR12R1     (*(volatile uint32_t *)(DAC_BASE + 0x08))

#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_ODR       (*(volatile uint32_t *)(GPIOB_BASE + 0x14))

void init_timer() {
    // Enable the TIM2 clock
    RCC_APB1ENR |= TIM2EN;

    // Set prescaler to divide the timer clock
    TIM2_PSC = 16 - 1;  // Assuming a 16 MHz clock, this sets the timer clock to 1 MHz

    // Set auto-reload value for a desired period
    TIM2_ARR = 44100 - 1;  // This sets the timer period to 1ms

    // Enable the capture/compare channel
    TIM2_CCER |= 1;

    // Enable the update interrupt
    TIM2_DIER |= 1;  // Enable overflow interrupt

    // Enable the TIM2 interrupt in the NVIC
    NVIC_ISER0 |= (1 << TIM2_IRQn);

    // Start the timer
    TIM2_CR1 |= 1;  // Enable the timer
}

void init_dac() {
    // enable GPIOA
    RCC_AHB1ENR |= 1;

    // set PA4 to analog mode
    GPIOA_MODER |= 0x300;

    // enable DAC
    RCC_APB1ENR |= 0x20000000;

    // enable DAC channel 1
    DAC_CR |= 1;

    // set data format but loading it into DAC_DHR12Rx right aligned 12b
    DAC_DHR12R1 = 0x000;
}

void init_gpio() {
    // IO port B clock enable
    RCC_AHB1ENR |= 2;

    // Set GPIOB pin 0 (user LED 1) to output
    GPIOB_MODER |= 0x4000;

}

int count = 0;
int sample_rate = 44100;
int f = 1000;
float phase = 0;
int amplitude = 0xFFF - 1;

int main(void) {
    init_dac();
    init_gpio();
    init_timer();
    // i = sinf(0);
    // i = sinf(0.5);
    // i = sinf(0.333);

    while (1) {
        // Main loop
    }
}

void TIM2_IRQHandler(void) {
    if (TIM2_SR & (1 << 1)) {  // Check if CC1 interrupt flag is set
        TIM2_SR &= ~(1 << 1);  // Clear the interrupt flag
        
        float x = 2*PI*(count+phase)/(sample_rate/f);
        x = sinf(x);
        uint32_t dac_value = (uint32_t)((x + 1.0) * 2047.5);
        DAC_DHR12R1 = dac_value;

        count++;
        if(count >= sample_rate) {
            count = 0;
        }
    }
}