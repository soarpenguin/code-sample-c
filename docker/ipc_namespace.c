#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
 
/* 定义一个给 clone 用的栈，栈大小1M */
#define STACK_SIZE (1024 * 1024)
static char container_stack[STACK_SIZE];
 
char* const container_args[] = {
    "/bin/bash",
    NULL
};

int container_main(void* arg)
{
    printf("Container - inside the container!\n");
    sethostname("container",10); /* 设置hostname */
    execv(container_args[0], container_args);
    printf("Something's wrong!\n");
    return 1;
}
 
int main()
{
    printf("Parent - start a container!\n");
    int container_pid = clone(container_main, container_stack+STACK_SIZE, 
            CLONE_NEWUTS | CLONE_NEWIPC | SIGCHLD, NULL); /*启用CLONE_NEWUTS Namespace隔离 */
    waitpid(container_pid, NULL, 0);
    printf("Parent - container stopped!\n");
    return 0;
}

//#########################################################################
// NO CLONE_NEWIPC
//➜  docker git:(master) ✗ ipcmk -Q
//Message queue id: 0
//➜  docker git:(master) ✗ ipcs -q
//
//------ Message Queues --------
//key        msqid      owner      perms      used-bytes   messages
//0x066ac6ed 0          fengye  644        0            0
//

//#########################################################################
// HAVE CLONE_NEWIPC
//➜  docker git:(master) ✗ sudo ./ipc
//Parent - start a container!
//Container - inside the container!
//root@container:/home/fengye/cprogram/code-sample-c/docker# ipcs
//
//------ Shared Memory Segments --------
//key        shmid      owner      perms      bytes      nattch     status
//
//------ Semaphore Arrays --------
//key        semid      owner      perms      nsems
//
//------ Message Queues --------
//key        msqid      owner      perms      used-bytes   messages
//
//root@container:/home/fengye/cprogram/code-sample-c/docker# ipcs -q
//
//------ Message Queues --------
//key        msqid      owner      perms      used-bytes   messages
