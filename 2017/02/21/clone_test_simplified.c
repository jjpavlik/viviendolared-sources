#include <stdio.h>
#include <errno.h>
#include <linux/sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>

#define STACK_SIZE (1024*1024)
#define THREADS 5

int myFunction(void *arg);

int main(int argc, char *argv[])
{
	int aux, i, childtid, array[THREADS];
	char *stack_low, *stack_high;

	for(i=0;i<THREADS;i++)
	{	
		*(array+i)=0;//Initializing array with zero, position by position, just for the fun of it (?
		
		//Allocate some bytes for the cloned child, since STACK 
		// grows downward we want to know the last memory position
		// to use it on clone().
		stack_low=malloc(STACK_SIZE);
		stack_high=stack_low+STACK_SIZE;
		
		//When CLONE_THREAD is set, CLONE_SIGHAND and CLONE_VM have to be set as well, have a look at https://linux.die.net/man/2/clone.
		childtid=clone(myFunction,(void *)stack_high,CLONE_THREAD|CLONE_VM|CLONE_SIGHAND,(void *)(array+i));
		aux=errno;
		if(childtid == -1)
		{
			printf("Clone failed: \"%s\"\n",strerror(aux));
			return 2;
		}
	}
	sleep(60);//We just wait... synching the threads is a pain... 
	
	//We print all the TID values stored in the array by every thread
	for(i=0;i<THREADS;i++)
	{
		printf("Spawned thread %d\n",*(array+i));
	}
	return 0;
}

// This function is used to run every new thread, it receives a pointer to the place where the thread should store its TID
int myFunction(void *arg)
{
	int *aux;
	aux=(int *)arg;
	*aux=syscall(SYS_gettid);//Stores the thread TID in the memory possition passed as argument.
	pause();
}
