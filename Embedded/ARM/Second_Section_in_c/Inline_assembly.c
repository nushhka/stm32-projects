#include <stdio.h>
#include <stm32f4xx.h>

void your_strcpy(const char *src , char *dst);

void your_strcpy(const char *src , char *dst){
__asm volatile(
		"loop:                     \n"
		" 	ldrb r1, [%0], #1      \n"
		" 	cbz r1, endloop        \n"
		" 	strb r1, [%1], #1      \n"
		" 	b loop                 \n"
		"endloop:                  \n"
		:
		: "r" (src), "r" (dst)
		: "r1","memory"
);
}

int main (void){
	const char a[] = "Hello world!";
	char b[20];
	your_strcpy(a, b);
	printf("Copied String: %s\n", b);
	return 0;
}
