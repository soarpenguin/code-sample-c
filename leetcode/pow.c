#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double 
pow(double x, int n) {
    // Start typing your C/C++ solution below
    // DO NOT write int main() function
    double result = 1;
    int minus = 1;

    if (n < 0) {
        minus = -1;
        n = -n;
    }

    if (0 == n) {
        return 1;
    } else if (0 == x) {
        return 0;
    }
    
    while (n) {
        if (n & 1) {
            result *= x;
        }
        x *= x;
        n /= 2;
    }

    if (minus < 0) {
        return 1.0 / result;
    } else {
        return result;
    }
}

int
main(int argc, char **argv) {
    return 0;
}
