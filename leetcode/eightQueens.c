#include <stdio.h>
#define true 1
#define false 0

int g_numbers = 0;

int
check(int *Queue, int length)
{
    int i = 0, j = 0;

    for(i = 0; i < length; ++i) {
        for(j = i + 1; j < length; ++j) {
            if(j-i == Queue[i]-Queue[j] || j-i == Queue[j]-Queue[i])
                return false;
        }
    }
    return true;
}

void
printQueue(int *Queue, int length)
{
    int i = 0, j = 0;
    
    printf("The solution %d is: \n", g_numbers);
    for(i = 0; i < length; ++i) {
        for(j = 0; j < length; ++j) {
            if(Queue[i] == j)
                printf(" 1");
            else
                printf(" 0");
        }
        printf("\n");
    }
}

void
Permutation(int *Queue, int length, int index)
{
    int temp;
    int i = 0;

    if(Queue == NULL || length <= 0)
        return;

    if(index == length) {
        if(check(Queue, length)) {
            ++g_numbers;
            printQueue(Queue, length);
        }
    } else {
        for(i = index; i < length; ++i) {
            temp = Queue[i];
            Queue[i] = Queue[index];
            Queue[index] = temp;

            Permutation(Queue, length, index+1);

            temp = Queue[i];
            Queue[i] = Queue[index];
            Queue[index] = temp;
        }
    }
}

void
EightQueue()
{
    const int queues = 8;
    int Queue[queues];
    int index = 0;

    for(index = 0; index < queues; ++index) {
        Queue[index] = index;
    }
    Permutation(Queue, queues, 0);
}

int
main(int argc, char **argv)
{
    EightQueue();

    return 0;
}

