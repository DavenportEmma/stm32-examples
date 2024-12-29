#include "stm32f722xx.h"
#include "uart.h"
/*
SPI1_SCK    PA5 PC10
SPI1_MISO   PA6 PC11
SPI1_MOSI   PA7 PC12
chip select is active low
vcc 3.3v
*/

void SPI_setup() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    GPIOA->MODER |= 0xA800;
    GPIOA->AFR[0] |= 0x55500000;

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
    GPIOC->MODER |= 0x02A00000;
    GPIOC->AFR[1] |= 0x66600;

    SPI1->CR1 = 0;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2;
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;
    SPI1->CR2 |= SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; // 16-bit data size
    SPI1->CR2 |= SPI_CR2_RXNEIE;
    SPI1->CR1 |= SPI_CR1_SPE;
    
    SPI3->CR1 = 0;
    SPI3->CR1 |= SPI_CR1_SSM;
    SPI3->CR2 |= SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; // 16-bit data size
    SPI3->CR2 |= SPI_CR2_RXNEIE;
    SPI3->CR1 |= SPI_CR1_SPE;

    NVIC_EnableIRQ(SPI1_IRQn);
    NVIC_EnableIRQ(SPI3_IRQn);
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


void main() {
    USART_Handler u;

    SPI_setup();
    UART_setup(&u);

    uint16_t message[] = {0x0001, 0x0002, 0x0003, 0x0004};
    int len = 4;
    for(int i = 0; i < len; i++) {
        while(!(SPI1->SR & SPI_SR_TXE)) {}
        SPI1->DR = message[i];
    }

    while(1) {

    }
}

void SPI1_IRQHandler(void) {
    
}

void SPI3_IRQHandler(void) {
    uint16_t r;
    if (SPI3->SR & SPI_SR_RXNE) {
        r = SPI3->DR;
        char m = (char)r;
        send_uart(USART3, (char*)&r, 2);
    }
}