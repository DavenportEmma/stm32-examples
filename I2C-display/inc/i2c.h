#ifndef _I2C_H
#define _I2C_H 

#include "stm32f722xx.h"

#define SEND_TIMEOUT 10000

#define GPIOF_AFRL      (*(volatile uint32_t *)(GPIOF_BASE + 0x20))

void init_i2c();
int start_i2c_tx(uint8_t address, uint8_t len);
void stop_i2c_tx();
int check_nack_i2c();
int send_byte_i2c(uint8_t address, uint8_t data);
int send_i2c(uint8_t address, uint8_t* data, uint8_t len);

#endif