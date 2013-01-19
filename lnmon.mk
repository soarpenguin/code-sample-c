CFLAGS=-g -D JFS -D GETUSER -Wall -D LARGEMEM
LDFLAGS=-lcurses
nmon: lnmon.o
	gcc -g ${CFLAGS} ${LDFLAGS} lnmon.c -o nmon
