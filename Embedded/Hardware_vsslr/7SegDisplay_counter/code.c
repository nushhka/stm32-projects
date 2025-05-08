#include <stm32f4xx.h>

int main(){
RCC->AHB1ENR |= 0x00000003; // A & B -> ENABLE
GPIOA->MODER |= 0x00055400; // (PA5-9)-> Output Mode
GPIOB->MODER |= 0x00050000; // (PB8 & PB9)-> Output Mode

GPIOB->ODR = 0x00000000; 
	
//SysTick ->LOAD = 0x00F42400; // 32 million
//SysTick ->VAL = 0; 
//SysTick ->CTRL = 0x5;

RCC->APB1ENR |= 1; // Enable clock for TIM2
TIM2->PSC = 15999;
TIM2->ARR = 1999;
TIM2->CNT = 0; // Clear Counter
TIM2->CR1 = 1; // Enable Timer 2
	
int i = 0;
//int f = 1;
while(1)
{
			
			while(!(TIM2->SR & TIM_SR_UIF)) {}
				
			i %=10;
			//if(SysTick->CTRL & 0x10000){   // (Check COUNTFLAG = 1)
//			if(f%2 == 0)
//			{
//				f++;
	//			continue;
		//	}
			//GPIOB->ODR = GPIOB->ODR ? 0x00004080; // Toggle
			if(i==0){
			GPIOA->ODR = 0x000003A0;
			GPIOB->ODR = 0x00000300;
			}
			else if(i == 1){
			GPIOA->ODR = 0x00000300;
			GPIOB->ODR = 0x00000000;
			}
			else if(i == 2){
			GPIOA->ODR = 0x00000160;
			GPIOB->ODR = 0x00000300;
			}
			else if(i == 3){
			GPIOA->ODR = 0x00000340;
			GPIOB->ODR = 0x00000300;
			}
			else if(i == 4){
			GPIOA->ODR = 0x000003C0;
			GPIOB->ODR = 0x00000000;
			}
			else if(i == 5){
			GPIOA->ODR = 0x000002C0;
			GPIOB->ODR = 0x00000300;
			}
			else if(i == 6){
			GPIOA->ODR = 0x000002E0;
			GPIOB->ODR = 0x00000300;
			}
			else if(i == 7){
			GPIOA->ODR = 0x00000300;
			GPIOB->ODR = 0x00000200;
			}
			else if(i == 8){
			GPIOA->ODR = 0x000003E0;
			GPIOB->ODR = 0x00000300;
			}
			else if(i == 9){
			GPIOA->ODR = 0x000003C0;
			GPIOB->ODR = 0x00000300;
			}
			i++;
//			f++;
			TIM2->SR &= (~ (TIM_SR_UIF));
			
			}

}
