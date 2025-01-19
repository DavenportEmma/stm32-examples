#include "stm32f722xx.h"
#include "uart.h"
#include "spi.h"

void setup() {
    /*
    setup uart for st link
    setup uart for midi
    setup spi flash chip
    setup gpio for keyboard
    */
   int err;

    USART_Handler u;
    u.uart = USART3;
    u.baud = 9600;
    u.gpio = GPIOD;
    u.tx_pin = 8;
    u.rx_pin = 9;
    u.afr_reg = 1;
    u.rx_interrupts = 0;

    err = init_uart(&u);
    if(err == 0) {
        send_uart(USART3, "ST-Link UART initialised\n", 25);
    }

    u.uart = USART1;
    u.baud = 31250;
    u.gpio = GPIOB;
    u.tx_pin = 6;
    u.rx_pin = 7;
    u.afr_reg = 0;
    u.rx_interrupts = 0;

    err = init_uart(&u);
    if(err) {
        send_uart(USART3, "Error initialising MIDI UART\n", 29);
    }

    SPI_Handler s;
    s.spi = SPI1;
    s.gpio = GPIOA;
    s.miso = 6;
    s.mosi = 7;
    s.clk = 5;
    s.cs = 4;

    err = init_spi(&s);
    if(err) {
        send_uart(USART3, "Error initialising SPI\n", 23);
    }

    uint8_t tx[] = {0x9F, 0x00, 0x00, 0x00};
    uint8_t rx[4];
    CS_low(GPIOA, 4);
    SPI_tx_rx(SPI1, tx, rx, 4);
    CS_high(GPIOA, 4);
    if(rx[1] != 0xEF || rx[2] != 0x40 || rx[3] != 0x18) {
        send_uart(USART3, "Error initialising SPI flash chip\n", 34);
    }

    send_uart(USART3, "Finished initialisation\n", 24);
}