/*  TAP - Tcpdump Analysing Parser
    a tcpdump parser for educational purposes
    Copyright (C) 2008 ithilgore - ithilgore.ryu.L@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Usage: tcpdump -X | ./tap */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define VERSION "0.6"

/* COLOUR CODES */
#define	RED  "[0;31;40m"
#define BLUE "[0;34;40m"
#define GREEN "[0;32;40m"

#define WC	GREEN	/* Window Colour "| + -" */
#define FC	RED	/* Field Colour */
#define VC	BLUE	/* Value Colour */

#define c2hex(c)	(isdigit(c)) ? ((c) - 0x30) : (((c) & 0x0f) + 0x09)
#define print_lim()	fprintf(stderr, WC "\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n")
#define print_lim2()	fprintf(stderr, WC "\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+")


/* FUNCTION DECLARATIONS */
inline int parse4(int, int);
void print_ip_steady(int, int);
void print_ip_options(void);
void print_tcp_steady(int, int);
void print_tcp_options(void);
void print_data(int term);


/* VARIABLES TYPEDEFS */

typedef struct info_ip {
	int protocol;	/* number for upper level protocol - tcp,udp,icmp etc */
	int total_len;	/* length of the whole datagram */
	int hlen;	/* header length - we see if there are ip options by this */
	int options;	/* bool var - true for existence of ip options */
	int left;	/* how many 4hexes(2 bytes) have been 
			   left after steady header (5 32bit words)
			   (for ip options) */
} info_ip;

typedef struct info_tcp {
	int hlen;
	int options;
	int left;
} info_tcp;


/* GLOBAL VARIABLES */

info_tcp tcp;	
info_ip ip;
char tok[5];	/* buffer-token for holding the 4hexes + '\0' */
char buf[10];	/* helper buffer for operations */

int main(void)
{

	int col = 0;	/* column in tcpdump - or 4hex */
	int line = 0;	/* row in tcpdump output */
	while(1) {
		if (parse4(line, col) < 0) { /* current packet ended */
			print_data(1);
			fprintf(stderr, "\n\n");
			col = 0;
			line = 0;
			memset(&ip, 0, sizeof(ip));
			parse4(line, col);	/* parse new packet */
		}		

		if ((line == 0) || (line == 1 && col < 2))
			print_ip_steady(line, col);
		else if (ip.options && ip.left)
			print_ip_options();
		else { /* here we do the switching between the rest of the protocols 
			* based on the number we get from the ip header - that is 
			* tcp,udp,icmp etc */
			print_tcp_steady(line, col);
		}

		col++;
		if (col == 8) {	/* new line */
			col = 0; 
			line++;
		}
	}
	return (EXIT_FAILURE);
}


int parse4(int line, int col) 
{
	char c;
	char temp[9];
	char buf[9];
	static int normal = 0;	/* var defining whether we are in the middle
				 * of parsing the 4hexes in a line */
	if (!normal) {
		while (1) {
			c = fgetc(stdin);
			if (c == '\n') {
				memset(temp, 0, sizeof(temp));
				fgets(temp, sizeof(temp), stdin);
				snprintf(buf, sizeof(buf), "\t0x00%d0:", line);
				if (!strncmp(temp, buf, sizeof(temp))) {
					normal++;
					fgetc(stdin);	/* clear out next whitespace */
					fgetc(stdin);	/* clear out next whitespace */
					break;
				}
			}
		}
	}			


	fgets(tok, sizeof(tok), stdin);	/* read 4 chars and null terminate tok[4] */
	fgetc(stdin);			/* clear out next whitespace */

	if (col == 7)
		normal = 0;


	/* The code lacks one end case where the end of datagram is at col 7.
	 * We have to find another way to define if the datagram has ended
	 * since checking if there is a space afterwards just wont to the
	 * job here. It has to know that its the end with another way
	 */

	if (strchr(tok, ' ') != NULL) {	/* we reached the end of datagram */
		normal = 0;
		return -1;
	}
	else 
		return 0;

}

void print_ip_options(void)
{
	static int i = 0;
	if (!i)
		fprintf(stderr, WC "|" FC "\t\t        Options: " VC "%s", tok);
	else {
		fprintf(stderr, VC " %s                      " WC "|", tok);
		print_lim();
	}
	ip.left--;
	if (!i)
		i++;
	else 
		i = 0;
}

void print_ip_steady(int line, int col)
{
	if (!line) {
		if (!col) {
			print_lim();
			fprintf(stderr, WC "|" FC "Ver: " VC "%c ", tok[0]);
			fprintf(stderr, WC "|" FC "IHL: " VC "%c ", tok[1]);

			ip.hlen = c2hex(tok[1]);
			/* if header length is > 5 (in 32bit words) then
			   we must take into account to view the additional 
			   options in the ip header */
			if (ip.hlen > 5) {
				ip.options++;
				ip.left = (ip.hlen - 5) << 1;
			}
			else 
				ip.options = 0;

			fprintf(stderr, WC "|" FC "   ToS: " VC "%s     ", &tok[2]);
		}
		else if (col == 1) {
			/* copy the next portion of XXXX inside buf 
			   take into account that there is a space between
			   each of the 4-char hex pairs that get formed */
			ip.total_len =	((((c2hex(tok[0]) & 0x0f) << 12) 
					| ((c2hex(tok[1]) & 0x0f) << 8)) 
					| ((c2hex(tok[2]) & 0x0f) << 4))
					| (c2hex(tok[3]) & 0x0f);

			fprintf(stderr, WC "|" FC "       Total Length: " VC "%s      " WC "|", tok);
			print_lim();
		}
		else if (col == 2) {
			fprintf(stderr, WC "|" FC "      Identification: " VC "%s     ",  tok);
		}
		else if (col == 3) {
			/* this (not really)obfu thing prints the 3bit flags of the ip header */
			char c = c2hex(tok[0]);
			fprintf(stderr, WC "|" FC "F: " VC "%x ", (c & 0x0e) >> 1);

			/* print the 13bit fragment offset */
			unsigned int t; 
			t = 	((((c & 0x01) << 12)
				| ((c2hex(tok[1]) & 0x0f) << 8)) 
				| ((c2hex(tok[2]) & 0x0f) << 4))
				| (c2hex(tok[3]) & 0x0f);
			fprintf(stderr, WC "|" FC "   Fragment Offset:" VC "%4x  " WC "|", t);
			print_lim();
		}
		else if (col == 4) {
			memset(buf, 0, sizeof(buf));
			strncpy(buf, tok, 2);
			fprintf(stderr, WC "|" FC "    TTL: " VC "%s    ", buf);
			fprintf(stderr, WC "|" FC "  Protocol: " VC "%s " WC "|", &tok[2]);
		}
		else if (col == 5) {
			fprintf(stderr, FC "     Header Checksum:  " VC "%s    " WC "|", tok);
			print_lim(); 
		}
		else if (col == 6) {
			strncpy(buf, tok, 4);
		}
		else if (col == 7) {
			buf[4] = ' ';
			strncat(buf, tok, 5);
			fprintf(stderr, WC "|" FC "\t\t    Source Address: " VC "%s \t\t        " WC "|", buf);
			print_lim();
		}
	}
	else if (line == 1) {	/* line = 1 */
		if (!col) {
			memset(buf, 0, sizeof(buf));
			strncpy(buf, tok, 4);
		}
		else if (col == 1) {
			buf[4] = ' ';
			strncat(buf, tok, 5);
			fprintf(stderr, WC "|" FC "\t\t    Destination Address: " VC "%s \t\t" WC "|", buf);
			print_lim();
		}
	}
}


int calc_matrix(int col) 
{
	/* We calculate some offsets that 
	 * the protocol above ip will need.
	 * The pattern goes sth like this: 
	 * Suppose this is a typical output from tcpdump
	 * consisting of 2 lines of 8 4hexes each
	 *
	 * XXXX  XXXX W XXXX XXXX C XXXX XXXX XXXX XXXX
	 * XXXX  XXXX   XXXX XXXX   XXXX XXXX XXXX XXXX
	 *
	 * We want to continue from the W point regardless
	 * of how many the ip options were.
	 * So for e.g if ip.hlen was 2 then it would have
	 * stopped at C so according to the lookup table
	 * we would need to subtract 2 to be back to where
	 * we were. The pattern is repeating itself with 
	 * the same values above 7, (thus mod 8 is used).
	 */

	static const int ltable[8] = {
		  0,	/* 0 */
		 -1,	/* 1 */
		 -2,	/* 2 */
		 -3,	/* 3 */
		 -4,	/* 4 */
		 -5,	/* 5 */
		  2,	/* 6 */
		  1,	/* 7 */
	};
	
	return (ltable[col % 8]);
}

void print_data(int term)
{
	static int j = 0;

	if (term && j) {
		/* closing the Data field */
		switch (j) {
			case 1:	
				fprintf(stderr, WC "\t\t\t\t\t\t|");
				break;
			case 2:
				fprintf(stderr, WC "\t\t\t\t\t|");
				break;
			case 3:
				fprintf(stderr, WC "\t\t\t\t|");
				break;
			case 4:
				fprintf(stderr, WC "\t\t\t\t|");
				break;
			case 5:
				fprintf(stderr, WC "\t\t\t|");
				break;
			case 6:
				fprintf(stderr, WC "\t\t|");
				break;
			case 7:
				fprintf(stderr, WC "\t\t|");
				break;
		}
		print_lim();
		j = 0;
	}
	else if (!term) {
		if (!j && strchr(tok, ' ') == NULL) {
			fprintf(stderr, WC "|" FC "\t    Data: " VC "%s ", tok);
			j++;
		}
		else if (j < 7) {
			fprintf(stderr, VC "%s ", tok);
			j++;
		}
		else {
			fprintf(stderr, VC "%s       " WC "|", tok);
			print_lim();
			j = 0;
		}
	}
}

void print_tcp_options(void)
{
	static int i = 0;
	if (!i) {
		fprintf(stderr, WC "|" FC "\t\t    TCP Options: " VC "%s", tok);
		i++;
	}
	else {
		fprintf(stderr, VC " %s                      " WC "|", tok);
		print_lim();
		i = 0;
	}
	tcp.left--;
}

void print_tcp_steady(int __line_, int __col_)
{

	/* To account for the possible loss of the lines eaten
	 * by the ip options we subtract the offset values
	 * we kept before, from the ones used here, so that
	 * the tcp header can be printed like the ip header
	 * was steady all the way here - we use calc_matrix()
	 * for that.
	 */
	int line = 0;
	int iphlen_4hex = ((ip.hlen - 0x5) << 1);

	if (iphlen_4hex > 5) {
		int i = 6;
		while(1) {
			if (iphlen_4hex >= i)
				line++;
			else 
				break;
			i += 7;
		}
	}
	line = __line_ - line;

	int col = __col_ + calc_matrix(iphlen_4hex);

	if (line == 1) {
		if (col == 2) {
			fprintf(stderr, WC "|" FC "      Source Port: " VC "%s        ", tok);
		}
		else if (col == 3) {
			fprintf(stderr, WC "|" FC "     Destination Port: " VC "%s    " WC "|", tok);
			print_lim();
		}
		else if (col == 4) {
			memset(buf, 0, sizeof(buf));
			strncpy(buf, tok, 4);
		}
		else if (col == 5) {
			buf[4] = ' ';
			strncat(buf, tok, 5);
			fprintf(stderr, WC "|" FC "\t\t    Sequence Number: " VC "%s \t\t\t" WC "|", buf);
			print_lim();
		}
		else if (col == 6) {
			memset(buf, 0, sizeof(buf));
			strncpy(buf, tok, 4);
		}
		else if (col == 7) {
			buf[4] = ' ';
			strncat(buf, tok, 5);
			fprintf(stderr, WC "|" FC "\t\t  Acknowledgment Number: " VC "%s \t\t" WC "|", buf);
			print_lim();
		}
	}
	else if (line == 2) {
		if (col == 0) {
			tcp.hlen = c2hex(tok[0]); 
			fprintf(stderr, WC "|" FC "DO: " VC "%x  ", tcp.hlen);
			if (tcp.hlen > 5) {
				tcp.options++;
				tcp.left = (tcp.hlen - 5) << 1;
			}
			else
				tcp.options = 0;

			/* Reserved (6 bits) */
			unsigned int t = c2hex(tok[1]) << 2;
			t = t | ((c2hex(tok[2]) & 0xc0) >> 6);
			fprintf(stderr, WC "|" FC "Reserved: " VC "%x", t);

			/* Flags (6 bits) */
			t = ((c2hex(tok[2]) & 0x03) << 4) | (c2hex(tok[3]) & 0x0f);
			fprintf(stderr, WC "|" FC " Flags: " VC "%2x ", t);
		}
		else if (col == 1) {
			fprintf(stderr, WC "|" FC "          Window: " VC "%s         " WC "|", tok);
			print_lim();
		}
		else if (col == 2) {
			fprintf(stderr, WC "|" FC "         Checksum: " VC "%s        ", tok);
		}
		else if (col == 3) {
			fprintf(stderr, WC "|" FC "      Urgent Pointer: " VC "%s     " WC "|", tok);
			print_lim();
		}
		else if (tcp.options && tcp.left) {
			print_tcp_options();
		}
	}
	else if (tcp.options && tcp.left) {
		print_tcp_options();
	}
	else {
		print_data(0);
	}

}
