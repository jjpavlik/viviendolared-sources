#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/prctl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define CHILDS 2
#define MEM 2*1024*1024

int main()
{
	int childs[CHILDS];
	int ctr,child,pid,ppid,pgrp,psid,aux;
	void *ptr;
	
	pid = getpid();
	pgrp = getpgrp();
	ppid = getppid();
	psid = getsid(0);
	printf("Parent PID = %d, PGRP = %d, PPID = %d, PSID = %d\n",pid,pgrp,ppid,psid);

	// Uncomment the following 4 lines to make the parent process ignore SIGCHLD, so the children won't become zombies when they finish.
	//sa.sa_handler = SIG_IGN;
	//sigaction(SIGCHLD,&sa,0);
	//sigemptyset(&sa.sa_mask);
	//sa.sa_flags = 0;
	
	for(ctr=0;ctr<CHILDS;ctr++)
	{
		child = fork();
		aux = errno;
		if(child != 0)
		{//Parent execution
			if(child == -1)
			{
				printf("Something caused fork() to fail, baaaad: %s\n",strerror(aux));
				return -1;
			}
		}
		else
		{//Child execution
			pid = getpid();
			pgrp = getpgrp();
			ppid = getppid();
			psid = getsid(0);
			printf("Child %d -> PID = %d, PGRP = %d, PPID = %d, PSID = %d\n",ctr,pid,pgrp,ppid,psid);
			ptr = malloc(MEM);
			sleep(15);
			return 3;
		}
	}
	sleep(30);

	return 0;
}
