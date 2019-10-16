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

void myfun(int signum)
{
	int i = 0;
	while(i < 10)
	{
		printf("process signal signum = %d\n",signum);
		sleep(1);
		i++;
	}
	return ;
}

int main()
{
	int i = 0;
	signal(14,myfun); //14) SIGALRM:时钟定时信号, alarm函数使用该信号.
	printf("alarm before\n");
	alarm(9);   		
	printf("alarm after\n");
	while(i < 20)
	{
		i++;
		sleep(1);
		printf("process things i = %d\n",i);
	}
	return 0;
}