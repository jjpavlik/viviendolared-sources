#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#define SIZE 2048

int main()
{
	int open_files[SIZE];
	int index=0;
	int i,keep_it;

	for(i=0;i<SIZE;i++)
	{
		printf("Opening file number %d:\n",i);
		open_files[i]=open("/etc/passwd",O_RDONLY);
		keep_it=errno;//we save errno before doing anything else
		if(open_files[i] == -1)
		{
			printf("%s\n",strerror(keep_it));//we print the system error that corresponds to errno
			return open_files[i];
		}
		printf("Opened file number %d, assigned FD=%d:\n",i,open_files[i]);
	}	
	printf("%d files have been opened.\n",SIZE);

	return 0;
}
