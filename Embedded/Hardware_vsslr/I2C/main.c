#include "stm32f4xx.h"
#include <stdio.h>
#include "i2c.h"

void delay(){
    for (volatile int i = 0; i < 1000000; i++); // Simple delay loop
}

int main(){
    I2C_config();
    uint8_t counter = 0;

    while(1){
        I2C_start();
        I2C_Address(0x40);
        I2C_Write(counter);
        I2C_stop();
        
        counter++; // Increment 8-bit counter
        delay();
    }
}
