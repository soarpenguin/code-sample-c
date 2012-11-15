/*
 * use backtrace get function call frame info
 * when have a SIGSEGV signal.
 * 
 * compile options:
 *	gcc -g -rdynamic backtrace.c 
 */

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* Obtain a backtrace and print it to stdout. */
void
print_trace (int sig)
{
	void *array[10];
	size_t size;
	char **strings;
	size_t i;

	size = backtrace (array, 10);
	strings = backtrace_symbols (array, size);

	printf ("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++)
		printf ("%s\n", strings[i]);

	free (strings);
	exit(1);
}

void func_c()
{
	*((volatile char *) 0x0) = 0x999;
}

void func_b()
{
	func_c();
}

void func_a()
{
	func_b();
}

int
main (void)
{
	signal(SIGSEGV, print_trace);

	func_a();

	return 0;
}
