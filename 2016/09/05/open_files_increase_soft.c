#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#define SIZE 2048
#define JUMP 100

int main()
{
	int open_files[SIZE];
	int index=0;
	int i,keep_it,aux;
	struct rlimit old, new;

	for(i=0;i<SIZE;i++)
	{
		printf("Opening file number %d:\n",i);
		open_files[i]=open("/etc/passwd",O_RDONLY);
		keep_it=errno;//we save errno before doing anything else
		if(open_files[i] == -1)
		{
			if(keep_it == 24)//Too many open files
			{
				printf("%s\n",strerror(keep_it));//we print the system error that corresponds to errno
				printf("Increasing NOFILE in %d\n",JUMP);
				getrlimit(RLIMIT_NOFILE,&old);
				printf("Current soft limit %d, current hard limit %d\n",(int)old.rlim_cur,(int)old.rlim_max);
				new.rlim_max=old.rlim_max;
				new.rlim_cur=old.rlim_cur+JUMP;
				aux=setrlimit(RLIMIT_NOFILE,&new);
				keep_it=errno;
				if(aux==0)
				{
					i=i-1;//reduce i in 1 to "move back" the loop one cycle.
				}
				else
				{
					printf("Couldn't raise the soft limit: %s\n",strerror(keep_it));
					return -1;
				}
			}
			else
			{//some different error
				return -1;
			}
		}
		else
		{
			printf("Opened file number %d, assigned FD=%d:\n",i,open_files[i]);
		}
	}	
	printf("%d files have been opened.\n",SIZE);

	return 0;
}
