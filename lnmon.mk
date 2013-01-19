CFLAGS=-g -D JFS -D GETUSER -Wall -D LARGEMEM #-Wunused-but-set-variable
LDFLAGS=-lcurses
nmon: lnmon.o
	gcc -g ${CFLAGS} ${LDFLAGS} lnmon.c -o nmon

clean:
	-rm *.o
	-rm nmon
