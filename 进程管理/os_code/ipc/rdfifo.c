#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
main()
{
	int fd,len;
	char buf[PIPE_BUF];
	mode_t mode=0666;
	
	if(mkfifo("fifo1",mode)<0)
	{
		printf("mkfifo() err\n");
		exit(1);
	}
	
	if((fd=open("fifo1",O_RDONLY))<0)
	{
		printf("pipe open err\n");
		exit(1);
	}
	while(len=(read(fd,buf,PIPE_BUF-1))>0)
	{
		printf("%s\n",buf);
	}
	close(fd);
}
