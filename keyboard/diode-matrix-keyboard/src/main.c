#include "stm32f722xx.h"
#include "autoconf.h"
#include "uart.h"
/*
DIN     PD7
CLK     PD6
CLR     PD5 active low

keyboard inputs
PC3
PC6
PC7
PC8
PC9
PC10
PC11
PC12

*/

#define CLR GPIO_ODR_OD5
#define CLK GPIO_ODR_OD6
#define DIN GPIO_ODR_OD7
#define DIN_OFFSET GPIO_ODR_OD7_Pos

#define ROWS 8
#define COLS 16

// pin numbers for each row
#define ROW_INPUT_0_PIN 3
#define ROW_INPUT_1_PIN 6
#define ROW_INPUT_2_PIN 7
#define ROW_INPUT_3_PIN 8
#define ROW_INPUT_4_PIN 9
#define ROW_INPUT_5_PIN 10
#define ROW_INPUT_6_PIN 11
#define ROW_INPUT_7_PIN 12

const int ROW_LUT[8] = {
    ROW_INPUT_0_PIN, ROW_INPUT_1_PIN, ROW_INPUT_2_PIN, ROW_INPUT_3_PIN,
    ROW_INPUT_4_PIN, ROW_INPUT_5_PIN, ROW_INPUT_6_PIN, ROW_INPUT_7_PIN
};

#define PREV_KEY_POS 0
#define PREV_KEY_MASK (1 << PREV_KEY_POS)
#define CHANGED_FLAG_POS 1
#define CHANGED_FLAG_MASK (1 << CHANGED_FLAG_POS)
#define CHANGE_DIRECTION_POS 2
#define CHANGE_DIRECTION_MASK (1 << CHANGE_DIRECTION_POS)

void setup() {
    // shift register
    // initialise gpio
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= 0x5400;
    GPIOD->ODR |= 0x0000;
    // pull CLR high
    GPIOD->ODR |= CLR;

    // column input pins
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    // port C defaults to input on all pins
    // enable pull down resistors
    GPIOC->PUPDR |= (2 << (ROW_INPUT_0_PIN * 2)) | (2 << (ROW_INPUT_1_PIN * 2)) |
                    (2 << (ROW_INPUT_2_PIN * 2)) | (2 << (ROW_INPUT_3_PIN * 2)) |
                    (2 << (ROW_INPUT_4_PIN * 2)) | (2 << (ROW_INPUT_5_PIN * 2)) |
                    (2 << (ROW_INPUT_6_PIN * 2)) | (2 << (ROW_INPUT_7_PIN * 2));
    

    USART_Handler* u;
    u->uart = USART3;
    u->baud = 9600;
    u->gpio = GPIOD;
    u->tx_pin = 8;
    u->rx_pin = 9;
    u->afr_reg = 1;
    u->rx_interrupts = 0;
    int flag = init_uart(u);
}

void clear() {
    GPIOD->ODR &= ~CLR;
    GPIOD->ODR |= CLR;
}

void loadBit(int b) {
    GPIOD->ODR |=  ((b << DIN_OFFSET) & DIN);
    GPIOD->ODR |= CLK;
    GPIOD->ODR &= ~(0x1 << DIN_OFFSET);
    GPIOD->ODR &= ~CLK;
}

uint16_t readCol(uint32_t col) {
    // read gpioc input register
    int d = GPIOC->IDR;
    // iterate over each row
    for(int i = 0; i < ROWS; i++) {
        // get key value by masking the associated pin and shifting it right
        int keyVal = (d >> ROW_LUT[i]) & 1;
        // get the previous value of the key
        int prevVal = (col >> ROW_LUT[i] & 1);

        if(keyVal != prevVal) {
            // if the current value and previous value are not the same
            if(keyVal == 0) {
                send_uart(USART3, "falling edge\n\r", 14);
            } else {
                send_uart(USART3, "rising edge\n\r", 13);
            }
        }
    }
    return d;
}

void scan(uint32_t m[COLS]) {
    loadBit(1);
    m[0] = readCol(m[0]);
    
    for(int i = 1; i < COLS; i++) {
        loadBit(0);
        m[i] = readCol(m[i]);
    }

    clear();
}

int main(void) {
    setup();
    uint32_t m[COLS] = {0};

    scan(m);
    scan(m);

    while(1) {

    }
}