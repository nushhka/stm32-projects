#include <stm32f4xx.h>
#include <stdint.h>
#include "iris_model.h"

void delay(volatile int time);
void init_gpio();
void all_leds_off();
void light_up(int class_id);
int predict_class(double *input);
int button_pressed();

#define RCC_AHB1ENR    (*(volatile uint32_t*)0x40023830)
#define GPIOB_MODER    (*(volatile uint32_t*)0x40020400)
#define GPIOB_ODR      (*(volatile uint32_t*)0x40020414)
#define GPIOC_MODER    (*(volatile uint32_t*)0x40020800)
#define GPIOC_IDR      (*(volatile uint32_t*)0x40020810)

void delay(volatile int time) {
    while (time--);
}

void init_gpio() {
    // Enable GPIOB and GPIOC clocks
    RCC_AHB1ENR |= (1 << 1);  // GPIOB
    RCC_AHB1ENR |= (1 << 2);  // GPIOC

    // Set PB0, PB7, PB14 as output
    GPIOB_MODER &= ~(0b11 << (2 * 0));    
    GPIOB_MODER |=  (0b01 << (2 * 0));

    GPIOB_MODER &= ~(0b11 << (2 * 7));    
    GPIOB_MODER |=  (0b01 << (2 * 7));

    GPIOB_MODER &= ~(0b11 << (2 * 14));   
    GPIOB_MODER |=  (0b01 << (2 * 14));

    // Set PC13 as input (reset value 00, so no need to configure)
}

void all_leds_off() {
    GPIOB_ODR &= ~(1 << 0);    // PB0
    GPIOB_ODR &= ~(1 << 7);    // PB7
    GPIOB_ODR &= ~(1 << 14);   // PB14
}

void light_up(int class_id) {
    all_leds_off();
    if (class_id == 0) GPIOB_ODR |= (1 << 0);     // Setosa -> PB0
    else if (class_id == 1) GPIOB_ODR |= (1 << 7); // Versicolor -> PB7
    else if (class_id == 2) GPIOB_ODR |= (1 << 14); // Virginica -> PB14
}

int predict_class(double *input) {
    double output[3];
    score(input, output); // Run model

    int max_idx = 0;
    if (output[1] > output[0]) max_idx = 1;
    if (output[2] > output[max_idx]) max_idx = 2;
    return max_idx;
}

int button_pressed() {
    return !(GPIOC_IDR & (1 << 13));  // Button pressed = PC13 is low
}

int main(void) {
    init_gpio();

    // List of test inputs (multiple test cases)
    double test_inputs[][9] = {
    {5.1, 3.5, 1.4, 0.2},   // 0 - Setosa
		{6.0, 2.2, 4.0, 1.0},   // 1 - Versicolor
		{6.9, 3.1, 5.4, 2.1},   // 2 - Virginica
    {4.9, 3.0, 1.4, 0.2},   // 0 - Setosa
    {5.0, 3.4, 1.5, 0.2},   // 0 - Setosa
    {5.9, 3.0, 4.2, 1.5},   // 1 - Versicolor
    {6.1, 2.8, 4.0, 1.3},   // 1 - Versicolor
    {7.2, 3.0, 5.8, 1.6},   // 2 - Virginica
    {6.7, 3.3, 5.7, 2.5}    // 2 - Virginica
};

    int num_tests = 9;
    int current = 0;

    while (1) {
        if (button_pressed()) {
            int result = predict_class(test_inputs[current]);
            light_up(result);

            current = (current + 1) % num_tests;

            // Simple debouncing
            while (button_pressed());
            delay(1000000);
        }
    }
}
