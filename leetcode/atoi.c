#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int
atoi(const char *str) {
    long long int result = 0;
    const char *tmp = str;
    int minus = 1;
    int base = 10;
    int eindex = 0;
    int eminus = 0;

    if (NULL == str) {
        return result;
    }

    while (isspace(*tmp)) {
        tmp++;
    }
    
    if ('-' == *tmp) {
        minus = -1;
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
    
    // XXX attention for (result * base) > INT_MAX:
    //      if result and base is int, then result * base < INT_MAX.
    //      compiler use the "int" store the tmp result.
    while (*tmp >= '0' && *tmp <= '9') {
        result = ((*tmp - '0') + result * base);
        tmp++;
    }

    result *= minus;

    if ( minus > 0 && result >= INT_MAX) {
        return INT_MAX;
    } else if (minus < 0 && result <= INT_MIN) {
        return INT_MIN;
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

    return (int)result;
}

int
main(int argc, char **argv) {
    const char *str = "    10522545459";

    int result = atoi(str);

    printf("The result: %d\n", result);
    return 0;
}
