#include <stdio.h>
#include <stm32f4xx.h>

void count_words(const char *str, int *word_count);

void count_words(const char *str, int *word_count) {
    __asm volatile(
        // Initialize r2 = str, r4 = word_count, r5 count, r6 for word detection
        "mov r2, %0                 \n" 	
        "mov r4, %1                 \n" 	
        "mov r5, #0                 \n" 	
        "mov r6, #0                 \n" 	

        // Count words by load byte from str and incr pointer, If null terminator, end loop
        "count_loop:                \n"
        "	ldrb r7, [r2], #1         \n" 	
        "	cbz r7, count_done        \n" 	

        // Check for space or start of a new word
        "	cmp r7, #' '              \n" 	
        "	beq space_detected        \n" 	
        "	mov r6, #1                \n" 	
        "	b count_loop              \n"

        "space_detected:            \n"
        "	cmp r6, #1                \n" 	// Check if a word was detected
        "	bne count_loop            \n" 	// If no word detected, continue
        "	add r5, r5, #1            \n" 	// Increment word count
        "	mov r6, #0                \n" 	// Reset word detection flag
        "	b count_loop              \n"

        "count_done:                \n"
        "	cmp r6, #1                \n" 	// Check if the last word was detected
        "	bne finish                \n"
        "	add r5, r5, #1            \n" 	// Increment word count

        "finish:                    \n"
        "	str r5, [r4]              \n" 	// Store word count
        :
        : "r" (str), "r" (word_count)
        : "r2", "r4", "r5", "r6", "r7", "memory"
    );
}



void reverse_str(const char *str, char *ans);

void reverse_str(const char *str, char *ans) {
__asm volatile(
        // Find the length of the string, r2=str, r3=counter
        "mov r2, %0 			      \n"        
        "mov r3, #0 						\n" 
				// Load byte and incr pointer, If null terminator, break loop else Incr length counter
        "find_len: 							\n"
        "	ldrb r4, [r2], #1 		\n"  		
        "	cbz r4, start_reverse \n" 		
        "	add r3, r3, #1 				\n"    
        "	b find_len 						\n"

        // Reverse copy the string
        "start_reverse: 				\n"
        "	sub r2, %0, #1 				\n"      // r2 = src + length - 1 (last)
        "	add r2, r2, r3 				\n"
        "	mov r5, %1 						\n"      // r5 = ans
        "reverseLoop: 					\n"
        "	ldrb r4, [r2], #-1 		\n"   	// Load byte and decrement pointer
        "	cbz r3, end_reverse 	\n"   	
        "	strb r4, [r5], #1 		\n"    	// Store byte in ans and increment pointer
        "	sub r3, r3, #1 				\n"     
        "	b reverseLoop 				\n"

        // Null-terminate the reversed string
        "end_reverse: 					\n"
        "	mov r4, #0 						\n"       
        "	strb r4, [r5] 				\n"       
        :
        : "r" (str), "r" (ans)
        : "r2", "r3", "r4", "r5", "memory"
    );
}

int main(void) {
    const char str[] = "a b c d testing reverse";
    char reversed[100];
		int word_count = 0;

		printf("Original String: %s\n",str);
	
    reverse_str(str, reversed);
    printf("Reversed String: %s\n", reversed);
		
    count_words(str, &word_count);  
    printf("Word Count: %d\n", word_count);
	
    return 0;
}
