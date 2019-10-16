#include "stdlib.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	pid = fork();
	if(pid == 0)	//���̴߳���
	{
		int i = 0;
		for(i=0;i<5;i++)
		{
			printf("this is child process i = %d\n",i);
			usleep(100);   //�������100ms
		}	
	}
	if(pid > 0)
	{
		int i = 0;
		for(i = 0;i< 5 ;i++)
		{
			printf("this is parent process i = %d\n",i);
			usleep(100); 	//�������100ms
		}
	}
	while(1);
	return 0 ;
}