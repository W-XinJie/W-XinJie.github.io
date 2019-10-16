#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include<pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int fd;
	char process_inter = 0; 
	int ret;
	ret=mkfifo("./myfifo",0777);
	if(ret < 0)
	{
		printf("mkfifo faild\n");
		return -1;
	}
	printf("mkfifo sucess\n");

	fd = open("/work/myfifo",O_WRONLY); 
	if(fd < 0)	
	{
		printf("open myfifo faild\n");
		return -2;
	}
	printf("open myfifo sucess \n");
	
	for(int i = 0;i<5;i++)
	{
		printf("this is first process i = %d\n",i);
		usleep(100);
	}
	process_inter = 1;
	
	write(fd,&process_inter,1);//往管道中写数据
	
	while(1);
	return 0;
}