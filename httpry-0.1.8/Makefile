#
#  ----------------------------------------------------
#  httpry - HTTP logging and information retrieval tool
#  ----------------------------------------------------
#
#  Copyright (c) 2005-2014 Jason Bittel <jason.bittel@gmail.com>
#

CC		= gcc
CCFLAGS  	= -Wall -O3 -funroll-loops -I/usr/include/pcap -I/usr/local/include/pcap
DEBUGFLAGS	= -Wall -g -DDEBUG -I/usr/include/pcap -I/usr/local/include/pcap
LIBS		= -lpcap -lm -pthread
PROG		= httpry
FILES		= httpry.c format.c methods.c utility.c rate.c

.PHONY: all clean

all: $(PROG)

$(PROG): $(FILES)
	$(CC) $(CCFLAGS) -o $(PROG) $(FILES) $(LIBS)

clean:
	rm -f $(PROG)
