#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include<pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/types.h"
#include "signal.h"
#include <sys/wait.h>

void myfun(int signum)
{
	int i = 0;
	while(i < 5)
	{
		printf("process signal signum = %d, i = %d\n",signum,i);
		sleep(1);
		i++;
	}
	return ;
}

void myfun1(int signum)
{
	printf("receive signum = %d\n",signum);
	wait(NULL);
	return ;
}

int main()
{
	pid_t pid;
	pid = fork();
	if(pid>0) //Ö÷½ø³Ì
	{
		int i = 0;
		signal(10,myfun);
		signal(17,myfun1);
		while(1);
		
	}
	if(pid == 0)
	{
		sleep(10);
		kill(getppid(),10);
		sleep(10);
		exit(0);
	}
	return 0;
}