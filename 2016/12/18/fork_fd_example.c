#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <string.h>
#define SIZE 250

int main()
{
	char buffer[SIZE],ch;
	int keep_it,childpid,aux,count;
	int my_pipe[2];//my_pipe[] will keep two FD, my_pipe[1] to write into the pipe and my_pipe[0] to read from the pipe.

	aux=pipe(my_pipe);//Note how the pipe is created BEFORE the fork() call
	
	if(aux==-1)//Check if pipe() failed
	{
		printf("Pipe failed due to \"%s\"\n",strerror(keep_it));//print the system error that corresponds to errno	
		return -1;
	}

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
		close(my_pipe[0]);//On the child process we can close the read end of the pipe
		printf("Hi, this is the child process, insert message here (:P less than %d letters please): ",SIZE);
		fgets(buffer,sizeof(buffer),stdin);
		count=write(my_pipe[1],buffer,SIZE);
		printf("message sent to parent process.\n");
	}
	else
	{//Parent code path here
		close(my_pipe[1]);//On the parent process we can close the write end of the pipe
		read(my_pipe[0],buffer,SIZE);
		printf("Parent process received message: %s",buffer);
	}
	return 1;
}
