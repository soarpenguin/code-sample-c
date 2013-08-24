#include <stdio.h>

void
Permutation2P(char *src, char *begin)
{
    char *pCh;
    char temp;

    if(NULL == src || NULL == begin)
        return;

    if(*begin == '\0')
        printf("%s\n", src);
    else{
        for(pCh = begin; *pCh != '\0'; ++pCh) {
            temp = *pCh;
            *pCh = *begin;
            *begin = temp;

            Permutation2P(src, begin+1);

            temp = *pCh;
            *pCh = *begin;
            *begin = temp;
        }
    }
}

void Permutation1P(char *src)
{
    if(NULL == src) {
        printf("\nNULL pointer!\n");
        return;
    } else
        Permutation2P(src, src);
}

int
main(int argc, char **argv)
{
    char src[] = "name";

    char *p = NULL;  // Attention for declear char* Pointer, set it NULL,
                    // else it not NULL

    Permutation1P(src);
    Permutation1P(p);
    p = src;
    Permutation1P(p);

    return 0;
}

