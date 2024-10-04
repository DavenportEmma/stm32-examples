#include "stm32f7xx.h" // Adjust this include to match your device-specific header file
#include <stdio.h>
#include <stdint.h>
#define SCB_CPACR       (*(volatile uint32_t *)(SCB_BASE + 0x88))
#define DWT_CTRL       (*(volatile uint32_t *)(DWT_BASE + 0x04))
#define DWT_CYCCNT       (*(volatile uint32_t *)(DWT_BASE + 0x04))
#define CoreDebug_DEMCR       (*(volatile uint32_t *)(CoreDebug_BASE + 0x0C))


void enableFPU(void) {
    // Enable the FPU
    SCB_CPACR |= ((3UL << 10*2) | (3UL << 11*2));
}

void enableDWT(void) {
    CoreDebug_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable DWT
    DWT->LAR = 0xC5ACCE55; 
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay(volatile uint32_t count) {
    while (count--) {
        __asm("NOP");
    }
}

int main(void) {
    // Enable the FPU
    enableFPU();
    
    // Enable the DWT cycle counter
    enableDWT();
    
    // Variables for floating-point operations
    float a = 1.234567f, b = 9.876543f, c;
    uint32_t start, end, cycles;

    // Perform a large number of floating-point operations
    start = DWT_CYCCNT; // Start the cycle counter
    for (int i = 0; i < 100000; ++i) {
        c = a * b;           // Perform the multiplication
        a = c + b;           // Perform addition
        b = c - a;           // Perform subtraction
    }
    end = DWT_CYCCNT;   // Stop the cycle counter

    // Calculate the number of cycles taken
    cycles = end - start;

    // Check the result and print the number of cycles taken

    printf("Result: %f, Cycles: %lu\n", c, cycles);
    
    while (1) {
        // Main loop
    }
}