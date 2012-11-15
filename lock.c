#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <syscall.h>

/* 
 * g++ lock.c -o test_free -D FREE -lpthread
 * g++ lock.c -o test_free -D FREE -lpthread
 * g++ lock.c -o test_fetchandadd -D FETCH_AND_ADD -lpthread
 * g++ lock.c -o test_mlock -D M_LOCK -lpthread
 * g++ lock.c -o test_cas -D CAS -lpthread
 * g++ lock.c -o test_cas_imp -D CAS_IMP –lpthread
 * g++ lock.c –o test_cas_imp_yield –D CAS_IMP_YIELD -lpthread 
 */
#if defined(__x86_64__)  
#define ATOMICOPS_WORD_SUFFIX "q"  //64位环境下使用cmpxchgq命令
#else
#define ATOMICOPS_WORD_SUFFIX "l"   //32位环境下使用cmpxchgl命令
#endif

static inline bool compare_and_swap(volatile size_t *p, size_t val_old, size_t val_new){
	char ret;
	__asm__ __volatile__("lock; cmpxchg" ATOMICOPS_WORD_SUFFIX " %3, %0; setz %1"//lock命令锁总线，因此可以保证多核同步
			: "=m"(*p), "=q"(ret)      //setz为ZF符号位是否置位，用于设置返回值
			: "m"(*p), "r" (val_new), "a"(val_old)
			: "memory");
	return (bool)ret;
}

static inline size_t fetch_and_add(volatile size_t *p, size_t add){
	unsigned int ret;
	__asm__ __volatile__("lock; xaddl %0, %1"
			:"=r" (ret), "=m" (*p)
			: "0" (add), "m" (*p)
			: "memory");
	return ret;
};

struct my_cas
{
	my_cas(unsigned char t):m_val_old(t){}
	size_t m_val_old;
	inline void try_continue(size_t val_old,size_t val_new){
		while(!compare_and_swap(&m_val_old,val_old,val_new)){};
	}
	inline void add(size_t val_new){
		fetch_and_add(&m_val_old,val_new);
	}
};

volatile size_t g_uCount;
pthread_mutex_t g_tLck=PTHREAD_MUTEX_INITIALIZER;
my_cas mutex(1);
const size_t cnt_num = 10000000;

void* 
sum_with_mutex_lock(void*)
{
	for(int i=0;i < cnt_num;++i) {
		pthread_mutex_lock(&g_tLck);
		g_uCount += 1;
		pthread_mutex_unlock(&g_tLck);
	}
}

void* 
sum_with_f_and_a(void*)  //用fetch_and_add原子操作来保证结果正确性。
{
	for(int i=0;i < cnt_num;++i) {
		fetch_and_add(&g_uCount,1);
	}
}

void* 
sum_with_cas(void*)  //用CAS原子操作来模拟锁操作。
{       
	for(int i=0;i< cnt_num;++i)
	{       
		mutex.try_continue(1,0);
		g_uCount += 1;
		mutex.try_continue(0,1);   
	}
}

void* 
sum_with_cas_imp(void*)
{
	for(int i=0;i< cnt_num;++i) {
		for(;;) {
			size_t u = g_uCount;
			if(compare_and_swap(&g_uCount,u,u+1)){   //在上一条语句和本条语句之间，g_uCount无篡改则进行加1，
				break;        //break出该循环，否则重试，直到成功。
			}
		}
	}
}

void* 
sum_with_cas_imp_yield(void*)
{
	for(int i=0;i< cnt_num;++i) {
		for(;;) {
			register size_t c = 1000; //
			while(c){
				size_t u = g_uCount;
				if(compare_and_swap(&g_uCount,u,u+1)){
					break;
				}
				c--;
			}
			if(!c){
				syscall(SYS_sched_yield); //增加一次让渡CPU的机会，spin lock通常应有这种策略
			}
		}
	}
}

void* 
sum_just_free(void*)
{       
	for(int i=0;i < cnt_num;++i) {  //完全无锁，无等待，但执行结果通常是错误的。
		g_uCount += 1;
	}
}

void* 
sum(void*)
{
#ifdef M_LOCK
	sum_with_mutex_lock(NULL);
#endif
#ifdef FETCH_AND_ADD
	sum_with_f_and_a(NULL);
#endif
#ifdef FREE
	sum_just_free(NULL);
#endif
#ifdef CAS
	sum_with_cas(NULL);
#endif
#ifdef CAS_IMP
	sum_with_cas_imp(NULL);
#endif
#ifdef CAS_IMP_YIELD
	sum_with_cas_imp_yield(NULL);
#endif
};

int 
main(int argc, char **argv)
{       
	pthread_t* thread = (pthread_t*) malloc(10*sizeof( pthread_t));
	for(int i=0;i<10;++i){       
		pthread_create(&thread[i],NULL,sum,NULL);
	}
	for(int i=0;i<10;++i){       
		pthread_join(thread[i],NULL);
	}
	printf("g_uCount:%d\n",g_uCount);
}

