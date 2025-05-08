#include "stm32f4xx.h"
#include "uart.h"
#include <stdio.h>

#define TRIG_PIN 5  // PA5 as Trigger
#define ECHO_PIN 7  // PA7 as Echo

void Timer2_Init(void);
void Delay_us(uint32_t us);
void Ultrasonic_Init(void);
uint32_t Ultrasonic_GetDistance(void);

volatile uint32_t pulse_width = 0;  // Global variable for debugging

void Timer2_Init(void) {
    RCC->APB1ENR |= (1 << 0);  // Enable TIM2 clock
    TIM2->PSC = 16 - 1;  // Prescaler for 1 MHz timer (1µs per tick)
    TIM2->ARR = 0xFFFFFFFF;  // Max auto-reload value
    TIM2->CR1 |= (1 << 0);  // Enable Timer
}

void Delay_us(uint32_t us) {
    TIM2->CNT = 0;
    while (TIM2->CNT < us);
}

void Ultrasonic_Init(void) {
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock

    // Configure PA5 as Output (Trigger)
    GPIOA->MODER |= (1 << (TRIG_PIN * 2));

    // Configure PA7 as Input (Echo)
    GPIOA->MODER &= ~(3 << (ECHO_PIN * 2));

    // Enable SYSCFG Clock
    RCC->APB2ENR |= (1 << 14);

    // Configure EXTI7 for PA7 (Echo)
    SYSCFG->EXTICR[1] &= ~(0xF << 12);
    SYSCFG->EXTICR[1] |= (0 << 12);
}

uint32_t Ultrasonic_GetDistance(void) {
    uint32_t pulse_counter = 0;

    // Send 10µs pulse
    GPIOA->ODR |= (1 << TRIG_PIN);
    Delay_us(10);
    GPIOA->ODR &= ~(1 << TRIG_PIN);

    // Wait for Echo Rising Edge
    while (!(GPIOA->IDR & (1 << ECHO_PIN)));

    // Start Counting Time
    pulse_counter = 0;
    while (GPIOA->IDR & (1 << ECHO_PIN)) {
        pulse_counter++;
        Delay_us(1);
    }

    // Store pulse width for debugging
    pulse_width = pulse_counter;

    // Calculate distance in cm
    return (pulse_counter * 0.0343) / 2;
}

int main(void) {
    UART3_Init();
    Timer2_Init();
    Ultrasonic_Init();

    while (1) {
        uint32_t distance = Ultrasonic_GetDistance();
        
        // Print Distance
        char buffer[30];
        sprintf(buffer, "Distance: %lu cm\r\n", distance);
        UART3_SendString(buffer);

        // Print Pulse Width for Debugging
        //sprintf(buffer, "Pulse Width: %lu us\r\n", pulse_width);
        //UART3_SendString(buffer);

        Delay_us(1000000);  // 1-second delay
    }
}
