#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int
atoi(const char *str) {
    const char *tmp = str;
    unsigned int retmp = 0;
    int minus = 0;
    int base = 10;
    int result = 0;
    int eindex = 0;
    int eminus = 0;

    if (NULL == str) {
        return result;
    }

    while (isspace(*tmp)) {
        tmp++;
    }
    
    if ('-' == *tmp) {
        minus = 1;
        tmp++;
    } else if ('+' == *tmp) {
        tmp++;
    }
    
    //if ('0' == *tmp) {
    //    if ('x' == *(tmp + 1) || 'X' == *(tmp + 1)) {
    //        base = 16;
    //        tmp += 2;
    //    } else {
    //        base = 8;
    //        tmp++;
    //    }
    //}
    while ('0' == *tmp) {
        tmp++;
    }
    
    while (*tmp >= '0' && *tmp <= '9') {
        retmp = ((*tmp - '0') + result * base);
        if ( !minus && retmp >= INT_MAX) {
            return INT_MAX;
        } else if (minus && -retmp <= INT_MIN) {
            return INT_MIN;
        } else {
            result = result * base + *tmp - '0';
            tmp++; 
        }
    }

    //if ('e' == *tmp || 'E' == *tmp) {
    //    tmp++;
    //    if ('-' == *tmp) {
    //        eminus = 1;
    //        tmp++;
    //    } else if ('+' == *tmp) {
    //        tmp++;
    //    }
    //    while (*tmp > '0' && *tmp < '9') {
    //        if ( !eminus && ((*tmp - '0') + eindex * 10) > INT_MAX) {
    //            return INT_MAX;
    //        } else if (eminus && -((*tmp - '0') + eindex * 10) < INT_MIN) {
    //            return INT_MIN;
    //        } else {
    //            result = result * base + *tmp - '0';
    //            tmp++; 
    //        }
    //    }
    //}
    if (minus) {
        result = -result;
    }

    return result;
}

int
main(int argc, char **argv) {
    const char *str = "2147483648";

    int result = atoi(str);

    printf("The result: %d\n", result);
    return 0;
}
