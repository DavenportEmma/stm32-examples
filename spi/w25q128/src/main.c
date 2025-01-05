#include "stm32f722xx.h"
#include "uart.h"
/*
SPI1_NSS    PA4
SPI1_SCK    PA5
SPI1_MISO   PA6
SPI1_MOSI   PA7
chip select is active low
vcc 3.3v
*/

void SPI_setup() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    GPIOA->MODER &= ~(0xFF00);
    GPIOA->MODER |= 0xA900;
    GPIOA->AFR[0] |= 0x55500000;
    GPIOA->PUPDR &= ~(3 << (6 * 2));
    GPIOA->PUPDR &= ~(3 << (7 * 2));
    GPIOA->PUPDR |= (1 << (6 * 2));
    GPIOA->PUPDR |= (1 << (7 * 2));

    GPIOA->ODR |= (1 << 4);

    SPI1->CR1 = 0;
    SPI1->CR2 = 0;
    // SPI1->CR2 |= SPI_CR2_SSOE;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;
    // SPI1->CR1 |= SPI_CR1_BR_2;
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2;
    // SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    SPI1->CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; // 8-bit data size
    SPI1->CR2 |= SPI_CR2_FRXTH;
    // SPI1->CR2 |= SPI_CR2_RXNEIE;
    SPI1->CR1 |= SPI_CR1_SPE;

    // NVIC_EnableIRQ(SPI1_IRQn);
}

void UART_setup(USART_Handler* u) {
    u->uart = USART3;
    u->baud = 9600;
    u->gpio = GPIOD;
    u->tx_pin = 8;
    u->rx_pin = 9;
    u->afr_reg = 1;
    u->rx_interrupts = 0;

    init_uart(u);
}

void SPI_tx_rx(uint8_t* tx, uint8_t* rx, int len) {
    GPIOA->ODR &= ~(1 << 4);  // CS low

    for(int i = 0; i < len; i++) {
        while(!(SPI1->SR & SPI_SR_TXE)) {}
        *(volatile uint8_t*)&SPI1->DR = tx[i];

        while(!(SPI1->SR & SPI_SR_RXNE)) {}
        rx[i] = SPI1->DR;

    }

    while(SPI1->SR & SPI_SR_BSY) {};
    GPIOA->ODR |= (1 << 4);  // CS high
}

void main() {
    USART_Handler u;
    SPI_setup();
    UART_setup(&u);

    uint8_t tx[] = {0x9F, 0x00, 0x00, 0x00};
    uint8_t rx[4];

    SPI_tx_rx(tx, rx, 4);
    send_uart(USART3, (char*)&rx, 4);
    


    while(1) {
    }
}

void SPI1_IRQHandler(void) {
    uint8_t r;
    if (SPI1->SR & SPI_SR_RXNE) {
        r = SPI1->DR;
        char m = (char)r;
        send_uart(USART3, (char*)&r, 1);
    }   
}