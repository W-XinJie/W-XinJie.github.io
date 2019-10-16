#include "stdlib.h"
#include <stdio.h>
#include <unistd.h>
#include<pthread.h>

void *fun(void *var)
{
	int j = 0;
	for(j = 0;j<10;j++)
	{
		usleep(100);
		printf("this is fun  j = %d\n",j);
	}
	
}

int main()
{
	char str[] = "hello linux \n";
	pthread_t tid;
	int ret;
	ret = pthread_create(&tid,NULL,fun,(void *)str);

	if(ret < 0)	
	{
		printf("creat thread faild\n");
		return -1;
	}

	for(int i = 0;i< 5 ;i++)
	{
		usleep(100); 	//¹ÒÆð½øÐÐ100ms
		printf("this is main fun i = %d\n",i);
		
	}

	while(1);
	return 0 ;
}