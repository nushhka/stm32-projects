#include <stdio.h>
#include <stm32f4xx.h>

int find_greater(int num1, int num2);

int find_greater(int num1, int num2) {
    int result;
    __asm volatile(
        "CMP %1, %2          \n"  // Compare num1 and num2
        "ITE GT              \n"  // If-Then-Else block
        "MOVGT %0, %1        \n"  // If num1 > num2, result = num1
        "MOVLE %0, %2        \n"  // Else, result = num2
        : "=r"(result)            // Output
        : "r"(num1), "r"(num2)    // Inputs
        : "cc"                    // Clobbers condition flags
    );
    return result;
}

int main(void) {
    int num1, num2, greater;  // Declare all variables at the top
    printf("Enter two numbers: ");
    scanf("%d %d", &num1, &num2);

    greater = find_greater(num1, num2);
    printf("The greater number is: %d\n", greater);

    return 0;
}
