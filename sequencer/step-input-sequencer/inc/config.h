#ifndef _CONFIG_H
#define _CONFIG_H

#include "stm32f722xx.h"

#define SEND_TIMEOUT 10000
#define SYS_CLK 16000000
#define STEP_MS 461     // ~130bpm
#define TEMPO_LED_FLASH_PERIOD 10

#define STLINK_UART USART3
#define STLINK_UART_GPIO GPIOD
#define STLINK_BAUD 9600
#define STLINK_UART_GPIO_TX_PIN 8
#define STLINK_UART_GPIO_RX_PIN 9

#define MIDI_UART USART1
#define MIDI_UART_GPIO GPIOB
#define MIDI_BAUD 31250
#define MIDI_UART_GPIO_TX_PIN 6
#define MIDI_UART_GPIO_RX_PIN 7

#define USER_LED_GPIO GPIOB
#define USER_LED_GPIO_PIN 14

#define RX_BUFFER_MAX 5
extern volatile uint8_t rx_buffer[RX_BUFFER_MAX];
extern volatile int rx_buffer_loc;
extern volatile int rx_buffer_ready;

#endif