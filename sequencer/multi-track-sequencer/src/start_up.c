#include "stm32f722xx.h"
#include "config.h"
#include "start_up.h"

void start_up() {
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