/*
 * gcctrace.c use backtrace get the function call frames
 * in code debug or source code reading.
 *
 * XXX:
 * use the gcc options like: 
 *	gcc -g -finstrument-functions -rdynamic -Wall
 *	remove "-O", add "-finstrument-functions" 
 *
 *  gcc -g -finstrument-functions -rdynamic -Wall uname.c gcctrace.c -o uname
 */

#include <stdio.h>
#include <string.h>
#include <execinfo.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/utsname.h>

static void* callstack[128];
static unsigned int offset = 0;
volatile static int result = 0;

void __attribute__((__no_instrument_function__))
print_stack()
{
    char* target = NULL;
    int i = 0;
    int frames = backtrace(callstack, 128);
    char** strs = backtrace_symbols(callstack, frames);
	struct utsname u_name;

	if(NULL == strs)
		return;
	//for (i = 0; i < frames; ++i) {
	//	printf(strs[i]);
	//}
	
	if (!result) {
		if((uname(&u_name) == -1)) {
			fprintf(stderr, "%s:uname(2)\n", strerror(errno));
			fprintf(stderr, "stack infomation may be error.\n");
			fprintf(stderr, "check backtrace_symbols return info for your system in [%s-%s].\n", __FILE__, __func__);
		} else if(strcmp(u_name.nodename, "ubuntu")) {
			fprintf(stderr, "check backtrace_symbols return info for your system in [%s-%s].\n", __FILE__, __func__);
		} 

		result = 1;
	}
    //target = strtok(strs[2]," ");
    //for (i=0;i<3;i++)
    //    target = strtok(NULL," ");
    //for(i=0;i<offset;i++)
    //    printf("| ");
    //printf("%d ",i);
    //puts(target);
	
	/* 
	 * backtrace_symbols return info strs in ubuntu like:
	 *		strs[2] = "./a.out(main+0x1e) [0x80488af]";
	 */
	target = strrchr((char *)strs[2], '/');

	if(NULL == target) 
		target = (char *)strs[2];
	else
		target++;

    for(i = 0; i < offset; i++)
        printf("| ");

    printf("%d ",i);
    puts(target);
    
	free(strs);
}

void __attribute__((__no_instrument_function__))
__cyg_profile_func_enter(void *this_fn,void* call_site)
{
    printf("in >> ");
    print_stack();
    offset++;
}

void __attribute__((__no_instrument_function__))
__cyg_profile_func_exit(void *this_fn,void* call_site)
{
    printf("out<< ");
    offset--;
    print_stack();
}

