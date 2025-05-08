#include "stm32f4xx.h"
#include <stdio.h>

void I2C_config();
void I2C_start();
void I2C_Write(uint8_t data);
void I2C_Address(uint8_t address);
void I2C_stop();
 
 void I2C_config(){
    //clock enabling
    RCC->APB1ENR |= (1<<21); //clock of I2C1
    RCC->AHB1ENR |= (1<<1); //clock for port B
    // port settings
    GPIOB->MODER = (2<<18)|(2<<16); //alternate functions for PB8 and PB9
    GPIOB->OTYPER = (1<<9)|(1<<8); // open drain for 8 and 9
    GPIOB->OSPEEDR = (3<<16)|(3<<18); //HIGH SPEED
    GPIOB->PUPDR |= (1<<16)|(1<<18); // pullup for 8 and 9
    // alternaete function selection
    GPIOB->AFR[1] = (4<<0)|(4<<4); //I2C for alternate function code 4
    //RESET I2C1
    I2C1->CR1 |= (1<<15); //RESET = 1
    I2C1->CR1 &= ~(1<<15); // RESET = 0
    //CLOCK SETTING
    I2C1->CR2 = 16; //PERIPHERAL CLOCK FREQUNECY = 42MHz
    I2C1->CCR = 80; //CLOCK CONTROL REGISTER IN STANDARD MODE
    I2C1->TRISE = 17; //1000/{1000/45} + 1
    //START CONDITION
    I2C1->CR1 |= (1<<0); //PERIPHERAL ENABLE
 }
 void I2C_start(void){
    I2C1->CR1 |=(1<<8); //start generation and ack
    while(!(I2C1->SR1 & (1<<0))); //check for the start condition generated
 }
 void I2C_Write(uint8_t data){
    while((I2C1->SR1 & (1<<7) == 0)); //WAIT UNTILL THE TXE IS 0
    I2C1->DR = data; //write data to the DR
    while(((I2C1->SR1 & (1<<2)) == 0)); //WAIT UNTILL THE BYTE TRANSFERRED IS FINISHED

 }
 void I2C_Address(uint8_t address){
    I2C1->DR = address;
    while(((I2C1->SR1 & (1<<1)) == 0)); //WAIT UNTILL THE ADDR is received
	 uint8_t temp = I2C1->SR1 | I2C1->SR2;
 }
 void I2C_stop(void){
    I2C1->CR1 |= (1<<9);
 }