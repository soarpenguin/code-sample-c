#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
addBinary(char *a, char *b) {
    // Start typing your C/C++ solution below
    // DO NOT write int main() function
    int lena = 0;
    int lenb = 0;
    int lsum = 0;
    int carry = 0;
    int index = 0;
    char *sum = NULL;

    if (NULL == a) {
        return b;
    } else if (NULL == b) {
        return a;
    }

    lena = strlen(a);
    lenb = strlen(b);
    lsum = lena > lenb ? (lena + 2) : (lenb + 2);
    
    sum = malloc((lsum) * sizeof(char));
    if (NULL == sum) {
        return NULL;
    }
    
    index = lsum - 1;

    while(lena > 0 && lenb > 0) {
        if ( '1' == a[lena - 1] && '1' == b[lenb - 1]) {
            sum[index] = carry ? '1' : '0';
            carry = 1;
        } else if (('1' == a[lena - 1] || '1' == b[lenb - 1])&& carry) {
            sum[index] = '0';
            carry = 1;
        } else if (carry) {
            sum[index] = '1';
            carry = 0;
        } else if ('0' == a[lena - 1] && '0' == b[lenb - 1]){
            sum[index] = '0';
            carry = 0;
        } else {
            sum[index] = '1';
            carry = 0;
        }

        lena -= 1;
        lenb -= 1;
        index -= 1;
    }

    if (lena <= 0 && lenb <= 0) {
        // do nothing;
    } else {
        if (lena > 0) {
            // get it from a;
        } else {
            // get it from b;
        }
    }

    if(index > 0) {
        // move front;
    }
    
    return sum;
}

int
main(int argc, char **argv) {
    return 0;
}
