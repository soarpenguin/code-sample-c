###Linux Namespace 有如下种类，官方文档在这里《Namespace in Operation》
    http://lwn.net/Articles/531114/

    分类	系统调用参数	相关内核版本
    1. Mount namespaces	CLONE_NEWNS	Linux 2.4.19
    2. UTS namespaces	CLONE_NEWUTS	Linux 2.6.19
    3. IPC namespaces	CLONE_NEWIPC	Linux 2.6.19
    4. PID namespaces	CLONE_NEWPID	Linux 2.6.24
    5. Network namespaces	CLONE_NEWNET	始于Linux 2.6.24 完成于 Linux 2.6.29
    6. User namespaces	CLONE_NEWUSER	始于 Linux 2.6.23 完成于 Linux 3.8)

    主要是:三个系统调用
    1. clone() – 实现线程的系统调用，用来创建一个新的进程，并可以通过设计上述参数达到隔离。
    2. unshare() – 使某进程脱离某个namespace
    3. setns() – 把某进程加入到某个namespace

