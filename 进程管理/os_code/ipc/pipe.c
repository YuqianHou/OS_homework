#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
main()
{
	pid_t pid;
	int fds[2];
	char buf1[50],buf2[50];

	pipe(fds);		/* Create a pipe */
	if ((pid=fork())<0)
	{
		printf("fork() error\n");
		exit(1);
	}
	else if (pid==0)	/* Child process */
	{
		close(fds[0]);
		sprintf(buf1,"data....\n");
		write(fds[1],buf1,50);
		exit(1);
	}
	else			/* Parent process*/
	{
		close(fds[1]);
		wait(0);
		read(fds[0],buf2,50);
		printf("I am parent, the message is %s\n",buf2);
	}
}
