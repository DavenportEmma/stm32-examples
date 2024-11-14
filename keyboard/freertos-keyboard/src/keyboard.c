#include "keyboard.h"

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