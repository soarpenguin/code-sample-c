#include <stdio.h>

// The result of print() is "The sum of num1 + num2 is:"
#define print(num1, num2) \
    printf("The sum of " #num1 " + " #num2 " is:", num1, num2)

void
addWithoutArithmetic(int num1, int num2)
{
    int sum = 0;
    int carry = 0;
    
    printf("The sum of %d + %d is:", num1, num2);

    sum = num1 ^ num2;
    // attention the priority of '<<' greater than '&' 
    carry = (num1 & num2) << 1; 

    while(num1 & num2) {
        num1 = sum;
        num2 = carry;

        sum ^= carry;
        carry = (num1 & num2) << 1;
    }

    printf(" %d\n", sum);
}

int
main(void)
{
    addWithoutArithmetic(3, 1);

    return 0;
}

