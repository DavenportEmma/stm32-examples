#ifndef _CONFIG_H
#define _CONFIG_H

#include "stm32f722xx.h"

#define SEND_TIMEOUT 10000
#define SYS_CLK 16000000
#define STEP_MS 115     // ~130bpm at 4 ppqn
#define TEMPO_LED_FLASH_PERIOD 10
#define PPQN 4
#define MAX_SEQUENCE_LENGTH 64
#define MAX_PRESCALER_VALUE 128

#define MIDI_UART USART1
#define MIDI_UART_GPIO GPIOB
#define MIDI_BAUD 31250
#define MIDI_UART_GPIO_TX_PIN 6
#define MIDI_UART_GPIO_RX_PIN 7

#define USER_LED_GPIO GPIOB
#define USER_LED_GPIO_PIN 14

#endif