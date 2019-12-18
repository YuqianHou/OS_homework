#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int count=0;
void ctrl_c_count(int);
main()
{
	int c;
	void (* old_handler)(int);
	old_handler=signal(SIGINT,ctrl_c_count);
	while ((c=getchar())!='\n');
	printf("Ctrl_C count=%d\n",count);
	signal(SIGINT,old_handler);
}
void ctrl_c_count(int dump)
{
	printf("Ctrl_C\n");
	count++;
}
