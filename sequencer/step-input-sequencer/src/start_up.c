#include "stm32f722xx.h"
#include "config.h"
#include "start_up.h"

void start_up() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;            // enable gpio d clock
    STLINK_UART_GPIO->AFR[1] |= 0x7 | (0x7 << 4);   // set alternate functions for PD8 and PD9
    RCC->APB1ENR |= (1 << 18);                      // enable clock for usart 3
    STLINK_UART_GPIO->MODER &= ~(0x3 << (STLINK_UART_GPIO_TX_PIN * 2) | 0x3 << (STLINK_UART_GPIO_RX_PIN * 2));  // Clear mode bits for gpio
    STLINK_UART_GPIO->MODER |= (2 << (STLINK_UART_GPIO_TX_PIN * 2)) | (2 << (STLINK_UART_GPIO_RX_PIN * 2));     // set alternate mode for pin
    RCC->DCKCFGR2 |= (1 << ((3 - 1) * 2));          // configure clock for usart 3
    RCC->CR |= RCC_CR_HSION;                        // set usart to high speed mode
    STLINK_UART->CR1 &= ~(0xFFFFFFFF);              // clear cr1
    STLINK_UART->BRR |= SYS_CLK / STLINK_BAUD;         // Assuming APB2 clock is 16 MHz

    STLINK_UART->CR1 |= (1 << 5);   // enable rx interrupts
    STLINK_UART->CR1 |= 0xC;        // enable tx and rx
    STLINK_UART->CR1 |= 0x1;        // enable usart
    NVIC_EnableIRQ(USART3_IRQn);

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    MIDI_UART_GPIO->AFR[0] |= 0x7000000;
    RCC->APB2ENR |= 0x10;
    MIDI_UART_GPIO->MODER &= ~(0x3 << (MIDI_UART_GPIO_TX_PIN * 2) | 0x3 << (MIDI_UART_GPIO_RX_PIN * 2));
    MIDI_UART_GPIO->MODER |= (2 << (MIDI_UART_GPIO_TX_PIN * 2)) | (2 << (MIDI_UART_GPIO_RX_PIN * 2));
    RCC->DCKCFGR2 |= (1 << ((1 - 1) * 2));
    RCC->CR |= RCC_CR_HSION;
    MIDI_UART->CR1 &= ~(0xFFFFFFFF);
    MIDI_UART->BRR |= SYS_CLK / MIDI_BAUD;

    MIDI_UART->CR1 |= 0x8;
    MIDI_UART->CR1 |= 0x1; // enable usart

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    USER_LED_GPIO->MODER |= (1 << (USER_LED_GPIO_PIN * 2));
    USER_LED_GPIO->ODR &= ~(1 << USER_LED_GPIO_PIN);
}