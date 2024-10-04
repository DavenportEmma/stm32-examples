#include "i2c.h"

// I2C_B_SCL    PF1     CN9_19      AF4
// I2C_B_SDA    PF0     CN9_21      AF4

void init_i2c() {
    RCC->AHB1ENR |= (1 << 5);   // enable clock for gpiof
    GPIOF->MODER |= 0xA;    // set alternate mode for pf0 and pf1
    GPIOF_AFRL |= 0x44;     // set alternate function 4 for pf0 and pf1
    GPIOF->OTYPER |= 0x3;   // open drain pf0 pf1
    GPIOF->OSPEEDR |= 0xA;  // pf0 pf1 high speed
    GPIOF->PUPDR |= 0x5;    // pf0 pf1 pull up resistors

    RCC->APB1ENR |= (1 << 22);  // set i2c peripheral clock
    I2C2->TIMINGR |= (3 << 28) | (4 << 20) | (2 << 16) | (0xC3 << 8) | 0xC7;
    I2C2->CR1 |= (1 << 0);  // set pe bit in cr1 register
}

int start_i2c_tx(uint8_t address, uint8_t len) {
    I2C2->CR2 |= (address << 1);
    I2C2->CR2 |= I2C_CR2_START;
    I2C2->CR2 |= (len << 16);

    while(!(I2C2->ISR & I2C_ISR_TXIS)) {
        if(I2C2->ISR & I2C_ISR_NACKF) {
            I2C2->CR2 |= I2C_CR2_STOP;
            return 1;
        }
    }
}

void stop_i2c_tx() {
    I2C2->CR2 |= I2C_CR2_STOP;
}

int check_nack_i2c() {
    if(I2C2->ISR & I2C_ISR_NACKF) {
        return 1;
    } else {
        return 0;
    }
}

int send_byte_i2c(uint8_t address, uint8_t data) {
    if(start_i2c_tx(address, 1) == 1) {
        return 1;
    }

    while(!(I2C2->ISR & I2C_ISR_TXIS)) {
        if(check_nack_i2c() == 1) {
            I2C2->CR2 |= I2C_CR2_STOP;
            return 1;
        }
    }

    I2C2->TXDR = data;

    while(!(I2C2->ISR & I2C_ISR_TC)) {
        if(check_nack_i2c() == 1) {
            I2C2->CR2 |= I2C_CR2_STOP;
            return 1;
        }
    }

    stop_i2c_tx();

    while (!(I2C2->ISR & I2C_ISR_STOPF)) {}

    return 0;
}

int send_i2c(uint8_t address, uint8_t* data, uint8_t len) {
    // set addressing mode in cr2
    // set slvae address in sadd bits
    // transfer direction
    // number of bytes to transmit
    // set start bit in cr2
    
    if(start_i2c_tx(address, len) == 1) {
        return 1;
    }

    for(int i = 0; i < len; i++) {
        uint32_t timeout_counter = 0;
        
        while(!(I2C2->ISR & I2C_ISR_TXIS)) {
            if(check_nack_i2c() == 1) {
                I2C2->CR2 |= I2C_CR2_STOP;
                return 1;
            }
        }
        I2C2->TXDR = data[i];
    }

    while(!(I2C2->ISR & I2C_ISR_TC)) {
        if(check_nack_i2c() == 1) {
            I2C2->CR2 |= I2C_CR2_STOP;
            return 1;
        }
    }

    stop_i2c_tx();

    while (!(I2C2->ISR & I2C_ISR_STOPF)) {}

    return 0;
}