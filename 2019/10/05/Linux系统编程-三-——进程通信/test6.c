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
	int i = 0;
	printf("pause before\n");
	pause();   		
	printf("pause after\n");
	while(i < 20)
	{
		i++;
		sleep(1);
		printf("process things i = %d\n",i);
	}
	return 0;
}