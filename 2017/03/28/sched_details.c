#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

void main()
{
	int sched,pri,pid,min,max,i,aux,error;
	struct sched_param params;

	pid = getpid();

	sched = sched_getscheduler(pid);
	pri = getpriority(PRIO_PROCESS,0);
	printf("PID=%d\n",pid);
	printf("Current scheduler is %d, current priority is %d\n",sched,pri);
	printf("Priorities MIN=%d, MAX=%d, nice=%d\n",sched_get_priority_min(sched), sched_get_priority_max(sched),getpriority(PRIO_PROCESS,0));	
	
	printf("Changing scheduling class to SCHED_FIFO\n");
	params.sched_priority=99;
	aux = sched_setscheduler(pid,SCHED_FIFO,&params);
	error=errno;
	if( aux == -1)
	{
		//You need to run this as root :D, otherwise you will get a permission denied
		printf("Setscheduler failed: %s\n",strerror(error));
		return;
	}
	sched = sched_getscheduler(pid);
	pri = getpriority(PRIO_PROCESS,0);
	printf("Scheduler is %d, current priority is %d\n",sched,pri);
	printf("Priorities MIN=%d, MAX=%d, nice=%d\n",sched_get_priority_min(sched), sched_get_priority_max(sched),getpriority(PRIO_PROCESS,0));	

	while(1)
	{//Inifinite loop
		i++;
	}
}
