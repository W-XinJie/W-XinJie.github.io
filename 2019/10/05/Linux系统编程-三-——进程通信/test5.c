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
int main()
{
	printf("raise before\n");
	raise(9);   			//_exit();	exit(); 退出次进程
	printf("raise after\n");
	return 0;
}