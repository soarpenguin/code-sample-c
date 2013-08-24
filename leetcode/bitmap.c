#include <limits.h>

#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITMASK(b) (1 << (b) % CHAR_BIT)
#define BITSLOTS(nb) (((nb) + CHAR_BIT - 1) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))


#include <stdio.h>
#include <string.h>
#define MAX 100

int
main(int argc, char **argv)
{
	char bitarray[BITSLOTS(MAX)];
	int index = 0;

	memset(bitarray, 0, BITSLOTS(MAX));

	for(index = 1; index <= MAX; index += 10) {
		BITSET(bitarray, index);
	}

	for(index = 1; index <= MAX; index++) {
		if(BITTEST(bitarray, index))
			printf("%d", 1);
		else
			printf("%d", 0);

		if(index % 10 == 0)
			printf("\n");
	}
	return 0;
}
