// System V IPC MessageQueue 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MSGKEY 75
 
struct msgform
{
	long mtype;
	char mtext[1024];
}msg;

int msgqid,i;

void CLIENT()
{
	int i;
	msgqid=msgget(MSGKEY,0777);
	for(i=10;i>=1;i--)
	{
		msg.mtype=i;
		printf("client send %d\n",msg.mtype);
		msgsnd(msgqid,&msg,1030,0);
	}
	exit(0);
}

void SERVER()
{
	long j;
	msgqid=msgget(MSGKEY,0777|IPC_CREAT);
	do
	{
		msgrcv(msgqid,&msg,1030,0,0);
		printf("server received %d\n",msg.mtype);
		for(j=0;j<1000000;j++);
	}while(msg.mtype!=1);
	msgctl(msgqid,IPC_RMID,0);
	exit(0);
}

int main()
{
	while((i=fork())==-1);
	if(!i) SERVER();
	while((i=fork())==-1);
	if(!i) CLIENT();
	wait(0);
	wait(0);
	return 0;
}
