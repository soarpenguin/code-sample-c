#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

int
main(int argc, char *argv[])
{
    int fd;

    if (argc < 3) {
        fprintf(stderr, "%s /proc/PID/ns/FILE cmd args...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);  /* Get file descriptor for namespace */
    if (fd == -1)
        errExit("open");

    if (setns(fd, 0) == -1)        /* Join that namespace */
        errExit("setns");

    execvp(argv[2], &argv[2]);     /* Execute a command in namespace */
    errExit("execvp");
}

////////// example usage.
//$ sudo ./uts_example container &
//[1] 14978
//clone() returned 14980
//uts.nodename in child:  container
//uts.nodename in parent: zhuyefeng
//
//$ ps aux | grep uts
//root     14978  0.0  0.0  71300  2236 pts/4    S    09:16   0:00 sudo ./uts_example container
//root     14979  0.0  0.0   5220   356 pts/4    S    09:16   0:00 ./uts_example container
//root     14980  0.0  0.0   5220    96 pts/4    S    09:16   0:00 ./uts_example container
//
//$ sudo ./ns_exec /proc/14979/ns/uts /bin/bash
//root@xxxxx# uname -n
//xxxxx
//$ sudo ./ns_exec /proc/14980/ns/uts /bin/bash
//root@container:~# uname -n
//container
