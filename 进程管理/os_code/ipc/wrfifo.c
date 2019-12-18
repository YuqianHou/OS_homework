#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
main()
{
	int fd,len,i;
	char buf[PIPE_BUF],buf1[PIPE_BUF];
	mode_t mode=0666;
	if ((fd=open("fifo1",O_WRONLY))<0)
	{
		printf("Pipe open err\n");
		exit(1);
	}
	while(1)
	{
		scanf("%s",buf1);
		len=sprintf(buf,buf1);
		write(fd,buf,len+1);
	}
	close(fd);
}
