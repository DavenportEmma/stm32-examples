#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "stm32f722xx.h"
/*
shift register control pins
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

void scan(uint32_t m[COLS]);
void clear();
void loadBit(int b);
uint16_t readCol(uint32_t col);

#endif