#include <stdio.h>

#define false 0
#define true 1

#define max 10
static int stack[max];
static int sp = 0;

// TODO attention is: stack's start index from 1, so it can store (max - 1) num
#define pop sp--
#define push(x) stack[++sp]=x

int
isStackPushPop(int *pushOrder, int *popOrder, int length)
{
    int i = 0, j = 0;
    int temp;

    for (; i < length && j < length; i++) {
        temp = pushOrder[i];

        if(pushOrder[i] != popOrder[j]) {
            push(temp);
            continue;
        } else {
            j++;
            while(sp > 0 && stack[sp] == popOrder[j]) {
                pop;
                ++j;
            }
        }
    }

    if(sp > 0)
        return false;
    else
        return true;
}


int
main(int argc, char **argv)
{
    int pushOrder[] = {2, 4, 6, 8, 9, 1};
    int popOrder[] = {4, 2, 6, 9, 1, 8};

    int length = (sizeof(pushOrder) / sizeof(int));
    // printf("Length is %d\n", length);

    if(isStackPushPop(pushOrder, popOrder, length))
        printf("True!\n");
    else
        printf("False!\n");

    return 0;
}

