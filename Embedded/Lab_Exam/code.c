#include <stm32f4xx.h>
void delay(int dd);
void delay(int dd) {
int i=0;
for (i=0; i<dd; i++) {
       __asm volatile("NOP");      // Insert a No Operation (NOP) instruction to waste CPU cycles
  }
}

int main(){
RCC->AHB1ENR |= 0x00000007; // A & B -> ENABLE
GPIOA->MODER |= 0x00055400; // (PA5-9)-> Output Mode
GPIOB->MODER |= 0x00050000; // (PB8 & PB9)-> Output Mode
GPIOB->MODER |= 0x00000000;
	
GPIOB->ODR = 0x00000000; 


	
//SysTick ->LOAD = 0x00F42400; // 32 million
//SysTick ->VAL = 0; 
//SysTick ->CTRL = 0x5;


delay(10000);
int a = GPIOC->IDR;
int b = GPIOB->IDR;
delay(1000);
int c = GPIOC->IDR;
int d = GPIOB->IDR;
delay(10000);
int a1 = 2^a + 2^b;
int a2 = 2^c + 2^d;
delay(10000);
int i = a2+a1;
//int f = 1;
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

}


