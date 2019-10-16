#include "stdlib.h"
#include <stdio.h>
#include <unistd.h>
#include<pthread.h>
#include <string.h>

int main()
{
	int fd[2];
	int ret;
	char writebuf[] = "hello linux"; 
	char readbuf[128] = {0};
	ret = pipe(fd);
	if(ret < 0)	
	{
		printf("creat pipe faild\n");
		return -1;
	}
	printf("creat pipe sucess fd[0] = %d,fd[1] = %d\n",fd[0],fd[1]);
	
	write(fd[1],writebuf,sizeof(writebuf));//往管道中写数据
	
	read(fd[0],readbuf,128);   //读管道中的数据
	printf("readbuf = %s\n",readbuf);
	
	/*memset(readbuf,0,128);//第二次读
	read(fd[0],readbuf,128);   //读管道中的数据
	printf("readbuf = %s",readbuf);	*/
	
	close(fd[0]);
	close(fd[1]);
	return 0 ;
}