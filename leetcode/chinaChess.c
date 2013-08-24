#include <stdio.h>
// way 1
#define HALF_BITS_LENGTH 4
#define FULLMASK    255
#define LMASK   (FULLMASK << HALF_BITS_LENGTH)
#define RMASK   (FULLMASK >> HALF_BITS_LENGTH)
#define RSET(b,n) (b = ((LMASK & b) ^ n))
#define LSET(b,n) (b = ((RMASK & b) ^ (n << HALF_BITS_LENGTH)))
#define RGET(b) (b & RMASK)
#define LGET(b) ((b & LMASK) >> HALF_BITS_LENGTH)
#define GRIDW   3
void
ChessOne() 
{
    unsigned char b;
    for(LSET(b,1); LGET(b) <= GRIDW * GRIDW; LSET(b, (LGET(b) + 1)))
        for(RSET(b, 1); RGET(b) <= GRIDW * GRIDW; RSET(b, (RGET(b) + 1)))
            if(RGET(b) % GRIDW != LGET(b) % GRIDW)
                printf("A = %d,\tB = %d\n", LGET(b), RGET(b));
}


// way2
struct {
    unsigned char a:4;
    unsigned char b:4;
} i;

void
ChessTwo()
{
    for(i.a = 1; i.a <= 9; i.a++) 
        for(i.b = 1; i.b <= 9; i.b++) 
            if(i.a % 3 != i.b % 3)
                printf("A = %d,\tB = %d\n", i.a, i.b);
}

// main
int 
main()
{
    printf("-------------Method One!----------------\n");
    ChessOne();
    
    printf("-------------Method Two!----------------\n");
    ChessTwo();
    return 0;
}

