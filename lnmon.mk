# NOTE: link the curent working copy of the code to lmon.c for compiling
CFLAGS=-g -O2 -D JFS -D GETUSER -Wall -D LARGEMEM -Wunused-but-set-variable
# CFLAGS=-g -O2 -D JFS -D GETUSER -Wall -D POWER
#CFLAGS=-g -D JFS -D GETUSER 
LDFLAGS=-lncurses -g
FILE=lnmon.c

#make nmon -f lnmon.mk

nmon:  $(FILE)
	cc -o nmon $(FILE) $(CFLAGS) $(LDFLAGS)
