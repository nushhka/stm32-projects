#include "stm32f412Zx.h"

void delay(int dd);
void EXTI15_10_IRQHandler(void);

int main() {
__disable_irq();
RCC->AHB1ENR |= 0x02; // Enable Clock Port B
RCC->AHB1ENR |= 0x4; // Enable Clock Port C
RCC->APB2ENR |= 0X4000; // Enable clock to SYSCFG
GPIOB->MODER |= 0x00004000; // Mode Register for PB7
GPIOB->MODER |= 0x10000000; // PB14
SYSCFG->EXTICR[3] = 0X0020; // Port C for EXT 13 INTERRUPT
EXTI->IMR |= 0x2000; // Unmask interrupt
EXTI->RTSR |= 0x2000; // Rising edge
NVIC_EnableIRQ(EXTI15_10_IRQn);
__enable_irq();
while(1) {
GPIOB->ODR |= 0x4000;  
delay(1000000);  
GPIOB->ODR &= ~0x4000; 
delay(1000000);
}
}

void EXTI15_10_IRQHandler(void) {
if (EXTI->PR & 0x2000) {  // Check if EXTI13 interrupt flag is set
        for (int i = 0; i < 2; i++) {  // Blink LED twice
            GPIOB->ODR |= 0X80;  
            delay(1000000);  
            GPIOB->ODR &= ~0X80; 
            delay(1000000);  
        }
        EXTI->PR = 0X2000;  
    }
	
//GPIOB->ODR = 0X80; // Turn on LED
//EXTI->PR = 0X2000;
}

void delay(int dd) {
    for (volatile int i = 0; i < dd; i++);  
}
