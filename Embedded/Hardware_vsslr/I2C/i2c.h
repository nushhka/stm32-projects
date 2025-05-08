#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"
#include <stdint.h>

// Function prototypes
void I2C_config();
void I2C_start();
void I2C_Write(uint8_t data);
void I2C_Address(uint8_t address);
void I2C_stop(void);

#endif // I2C_H
