#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc, char **argv)
{
        unsigned long size,i;
        int N,S;
        N=atoi(argv[1]);
        S=atoi(argv[2]);
        size=(1024*1024*(unsigned long)N);
        char *a;
        a=malloc(size);
        if(a==NULL && errno == ENOMEM)
        {
                printf("Memoria insuficiente para asignar %ld Bytes\n",size);
                return -1;
        }
        for(i=0;i<size;i++)
	{
                *(a+i)=0;
        }
        sleep(S);
        printf("%ld Bytes\n",size);
        free(a);
        return 0;
}
