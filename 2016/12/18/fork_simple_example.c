#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <string.h>

int main()
{
	int pid,ppid,childpid,keep_it;
	pid=getpid();//Get process ID
	ppid=getppid();//Get parent process ID
	
	childpid=fork();//fork() returns the child PID on the parent's code path and 0 on the child's. On failure returns -1
	//From this point on, there are 2 processes running, unless fork failed of course :D.
	keep_it=errno;
	if(childpid==-1)//Check if fork() failed
	{
		printf("Fork failed due to \"%s\"\n",strerror(keep_it));//print the system error that corresponds to errno	
		return -1;
	}

	if(childpid==0)//Here is where the paths change, it could be done differently.
	{//Child code path here
		printf("Child process: \nPID\tPPID\n%d\t%d\n",pid,ppid);
		pid=getpid();
		ppid=getppid();
		printf("Child process: \nPID\tPPID\n%d\t%d\n",pid,ppid);
		sleep(5);
	}
	else
	{//Parent code path here
		sleep(10);
		printf("Parent process: \nPID\tPPID\n%d\t%d\n",pid,ppid);
		printf("Parent process: child PID was %d\n",childpid);
	}
	return 1;
}
