#include "stm32f722xx.h"
#include "midi.h"
// pd8
#define GPIOB_AFRL      (*(volatile uint32_t *)(GPIOB_BASE + 0x20))

#define SEND_TIMEOUT 10000

// if 1 then observe the output on the COM port on your computer
// if 0 then observe the output on pin pb6
#define STLINK_UART 0
#define BAUD_RATE 31250
#define SYS_CLK 16000000

void init_uart(
    USART_TypeDef* uart,
    uint16_t uartNum,
    GPIO_TypeDef* gpio,
    uint16_t pin
) {
    if(STLINK_UART == 0) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN_Msk;
        gpio->AFR[0] |= 0x7000000;
        RCC->APB2ENR |= 0x10;
    } else {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN_Msk;// enable clock for gpiod
        GPIOD->AFR[1] |= 0x7;                   // set pd8 to af7
        RCC->APB1ENR |= (1 << 18);           // Enable clock for USART3
    }

    gpio->MODER &= ~(0x3 << (pin * 2));    // Clear mode bits for pd8
    gpio->MODER |= 2 << (pin * 2);         // set alternate mode for pb8

    RCC->DCKCFGR2 |= (1 << ((uartNum - 1) * 2));
    RCC->CR |= RCC_CR_HSION;

    uart->CR1 &= ~(0xFFFFFFFF);   // clear cr1
    uart->BRR |= SYS_CLK / BAUD_RATE;  // Assuming APB2 clock is 16 MHz
    uart->CR1 |= 0x1; // enable usart
    uart->CR1 |= 0x8; // enable tx
}

MIDIError_TypeDef send(USART_TypeDef* uart, char* msg, int len) {
    for(int i = 0; i < len; i++) {
        uint32_t timeout_counter = 0;

        while(!(uart->ISR & (1 << 7))) {
            timeout_counter++;
            if(timeout_counter >= SEND_TIMEOUT) {
                return SEND_ERR_TIMEOUT;
            }
        }
        uart->TDR = msg[i];
    }
    return NO_ERROR;
}

void packet(
    char* buffer,
    uint16_t len,
    MIDIStatus_TypeDef status,
    MIDIChannel_TypeDef channel,
    MIDINote_TypeDef note,
    char velocity
) {
    buffer[0] = status | channel;
    buffer[1] = note;
    buffer[2] = velocity;
}

int main(void) {
    MIDIError_TypeDef flag;
    USART_TypeDef* u;
    uint16_t uartNum;
    GPIO_TypeDef* g;
    uint16_t uartPin;

    if(STLINK_UART == 0) {
        u = USART1;
        uartNum = 1;
        g = GPIOB;
        uartPin = 6;
    } else {
        u = USART3;
        uartNum = 3;
        g = GPIOD;
        uartPin = 8;
    }

    init_uart(u, uartNum, g, uartPin);
    
    uint16_t len = 3;
    char msg[len];
    for(uint32_t i = 0; i < 1000000; i++) {}
    packet(msg, len, NOTE_ON, CHANNEL_2, C4, 0x5F);
    flag = send(u, msg, len);
    for(uint32_t i = 0; i < 1000000; i++) {}
    packet(msg, len, NOTE_OFF, CHANNEL_2, C4, 0x5F);
    flag = send(u, msg, len);
    packet(msg, len, NOTE_ON, CHANNEL_2, A4, 0x5F);
    flag = send(u, msg, len);
    for(uint32_t i = 0; i < 1000000; i++) {}
    packet(msg, len, NOTE_OFF, CHANNEL_2, A4, 0x5F);
    flag = send(u, msg, len);

    while(1) {
    }
}