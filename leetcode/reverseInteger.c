#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
reverse(int x) {
    int minus = 1;
    long long result = 0;
    long tmp = 0;

    if (x < 0) {
        minus = -1;
        x = -x;
    }

    while (x) {
        tmp = x % 10;
        result = result * 10 + tmp;

        if (minus > 0) {
            if (result >= INT_MAX) {
                return INT_MAX;
            }
        } else {
            if (-result <= INT_MIN) {
                return INT_MIN;
            }
        }

        x /= 10;
    }

    return minus * result;
}

int
main(int argc, char **argv) {
    return 0;
}
