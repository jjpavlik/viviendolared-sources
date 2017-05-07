#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_CON 25
#define SERVER_PORT 8080
#define SERVER "192.168.0.26"

int main()
{
	int ret_code,aux,i;
	int opened_fd[MAX_CON];

	struct sockaddr_in dest_addr;
	struct in_addr addr;

	
	//Create in_addr structure with the Server IP
	ret_code = inet_aton(SERVER,&addr);//
	if(ret_code == 0)
	{
		printf("inet_aton failed to convert IP to addr\n");
		return -1;
	}
	
	//Create the dest_addr structure with 
	memset(&dest_addr,'\0',sizeof(dest_addr));//zero out the structure
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	bcopy((char *)&addr,(char *)&dest_addr.sin_addr,sizeof(addr));


	for(i=0;i<MAX_CON;i++)
	{
		//Open an IPv4 socket(AF_INET), TCP (SOCK_STREAM), socket returns a FD or -1 on error.
		ret_code=socket(AF_INET,SOCK_STREAM, 0);
		aux=errno;//capture errno value right after the call
		if(ret_code == -1)
		{//Something went wrong, use errno to check
			printf("Socket call failed due to: %s\n",strerror(aux));
			return -1;
		}
		opened_fd[i]=ret_code;
		printf("Socket opened FD=%d, connection number %d\n",ret_code,i);

		ret_code=connect(opened_fd[i],(struct sockaddr *)&dest_addr,sizeof(dest_addr));
		aux=errno;
		if(ret_code == -1)
		{
			printf("Connect failed due to: %s\n",strerror(aux));
			return -1;
		}
		ret_code = write(opened_fd[i],"juan",4);
		aux=errno;
		if(ret_code != 4)
		{
			printf("Write failed due to: %s\n",strerror(aux));
			return -1;
		}
		sleep(1);
	}
	sleep(120);
}
