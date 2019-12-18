# 经典IPC问题实现作业报告

<center>软工2班 3017218092 侯雨茜</center>



###一、运行实例代码

进入文件目录，编译运行

```shell
cd /home/yuqianhou/文档/pc_semaphore
make
./init
```

先运行生产者：

```shell
./producer
```

再新建一个shell，运行消费者：

```shell
./consumer
```

运行结果如下：

![屏幕快照 2019-10-30 09.46.28](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/os_code/ipc/pc_semaphore/截图/屏幕快照 2019-10-30 09.46.28.png)

![屏幕快照 2019-10-30 09.47.33](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/os_code/ipc/pc_semaphore/截图/屏幕快照 2019-10-30 09.47.33.png)

![屏幕快照 2019-10-30 09.48.18](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/os_code/ipc/pc_semaphore/截图/屏幕快照 2019-10-30 09.48.18.png)



### 二、读者优先描述

在几个经典IPC问题中，我选择了读者写者问题中的读者优先进行编程。

读者优先描述如下：

如果读者来：

1. 无读者、写者，新读者可以读；
2. 无写者等待，但有其他读者正在读，新读者可以读；
3. 有写者等待，但有其他读者正在读，新读者可以读；
4. 有写者写，新读者等

如果写者来：

1. 无读者，新写者可以写；
2. 有读者，新写者等待；
3. 有其他写者写或等待，新写者等待



### 三、增加读者和写者

新建`reader.c`文件，编写如下代码：

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "myipc.h"

int main(int argc, char *argv[])
{
    int i,item,shmid;
	// semaphore mutex,empty,full;
    semaphore fmutex = 1;
    semaphore rdcntmutex = 1;
    int reader_count = 0;
    union semun sem_union;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;

	if ( (fmutex = semget((key_t)KEY_MUTEX,1,0666|IPC_CREAT)) == -1 ) {
		fprintf(stderr,"Failed to create semaphore!"); 
		exit(EXIT_FAILURE);
	}
	if ( (rdcntmutex = semget((key_t)KEY_EMPTY,1,0666|IPC_CREAT)) == -1 ) {
		fprintf(stderr,"Failed to create semaphore!"); 
		exit(EXIT_FAILURE);
	}
	// if ( (full = semget((key_t)KEY_FULL,1,0666|IPC_CREAT)) == -1 ) {
	// 	fprintf(stderr,"Failed to create semaphore!"); 
	// 	exit(EXIT_FAILURE);
	// }
	if ( (shmid = shmget((key_t)KEY_SHM,sizeof(struct shared_use_st),0666|IPC_CREAT)) == -1 ) {
		fprintf(stderr,"Failed to create shared memory!"); 
		exit(EXIT_FAILURE);
	}

	if ( (shared_memory = shmat(shmid,(void *)0,0) ) == (void *)-1) {
		fprintf(stderr,"shmat failed\n");
		exit(EXIT_FAILURE);
	}
	shared_stuff = (struct shared_use_st *)shared_memory;

    for (i = 0; i < 30; i++)
    {
        item = ++(shared_stuff->cur);
		sleep(1);
		printf("Reading %d\n",item);

        sem_p(rdcntmutex);
        if (reader_count == 0)
        {
            sem_p(fmutex);
        }
        reader_count += 1;
        sem_v(rdcntmutex);

        // Do read operation
        (shared_stuff->buffer)[(shared_stuff->hi)]=item;
		(shared_stuff->hi) = ((shared_stuff->hi)+1) % BUFFER_SIZE;
		printf("Inserting item %d\n",item);

        sem_p(rdcntmutex);
        reader_count -= 1;
        if (reader_count == 0)
        {
            sem_v(fmutex);
        }
        sem_v(rdcntmutex);
         
    }

    if (shmdt(shared_memory) == -1) {
       		fprintf(stderr, "shmdt failed\n"); 
		exit(EXIT_FAILURE);
	}
	printf("Finish!\n");
	getchar();
  	exit(EXIT_SUCCESS);
}
```



新建`writer.c`文件，编写如下代码：

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "myipc.h"

int main(int argc, char *argv[])
{
    int i,item,shmid;
	// semaphore mutex,empty,full;
    semaphore fmutex = 1;
    semaphore rdcntmutex = 1;
    int reader_count = 0;
    union semun sem_union;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;

	if ( (fmutex = semget((key_t)KEY_MUTEX,1,0666|IPC_CREAT)) == -1 ) {
		fprintf(stderr,"Failed to create semaphore!"); 
		exit(EXIT_FAILURE);
	}
	if ( (rdcntmutex = semget((key_t)KEY_EMPTY,1,0666|IPC_CREAT)) == -1 ) {
		fprintf(stderr,"Failed to create semaphore!"); 
		exit(EXIT_FAILURE);
	}
	// if ( (full = semget((key_t)KEY_FULL,1,0666|IPC_CREAT)) == -1 ) {
	// 	fprintf(stderr,"Failed to create semaphore!"); 
	// 	exit(EXIT_FAILURE);
	// }
	if ( (shmid = shmget((key_t)KEY_SHM,sizeof(struct shared_use_st),0666|IPC_CREAT)) == -1 ) {
		fprintf(stderr,"Failed to create shared memory!"); 
		exit(EXIT_FAILURE);
	}

	if ( (shared_memory = shmat(shmid,(void *)0,0) ) == (void *)-1) {
		fprintf(stderr,"shmat failed\n");
		exit(EXIT_FAILURE);
	}
	shared_stuff = (struct shared_use_st *)shared_memory;

    for (i = 0; i < 30; i++)   
    {
        sem_p(fmutex);

        // Do write operation
        item = shared_stuff->buffer[shared_stuff->lo];
		(shared_stuff->buffer)[(shared_stuff->lo)]=0;
		(shared_stuff->lo) = ((shared_stuff->lo)+1) % BUFFER_SIZE;
		printf("Removing item %d\n",item);
        sem_v(fmutex);
        
		printf("Writing %d\n",item);
		sleep(2);
    }

    if (shmdt(shared_memory) == -1) {
       		fprintf(stderr, "shmdt failed\n"); 
		exit(EXIT_FAILURE);
	}
	printf("Finish!\n");
	getchar();
  	exit(EXIT_SUCCESS);
}
```



修改init.c文件，使其能初始化reader和writer：

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "myipc.h"

int main(int argc, char *argv[])
{
    // int i,producer_pid,consumer_pid,item,shmid;
	int i,reader_pid,writer_pid,item,shmid;
	// semaphore mutex,empty,full;
	semaphore fmutex = 1;
    semaphore rdcntmutex = 1;
    union semun sem_union;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;

	if ( (fmutex = semget((key_t)KEY_MUTEX,1,0666|IPC_CREAT)) == -1 ) {
		fprintf(stderr,"Failed to create semaphore!"); 
		exit(EXIT_FAILURE);
	}
	if ( (rdcntmutex = semget((key_t)KEY_EMPTY,1,0666|IPC_CREAT)) == -1 ) {
		fprintf(stderr,"Failed to create semaphore!"); 
		exit(EXIT_FAILURE);
	}
	// if ( (full = semget((key_t)KEY_FULL,1,0666|IPC_CREAT)) == -1 ) {
	// 	fprintf(stderr,"Failed to create semaphore!"); 
	// 	exit(EXIT_FAILURE);
	// }
	if ( (shmid = shmget((key_t)KEY_SHM,sizeof(struct shared_use_st),0666|IPC_CREAT)) == -1 ) {
		fprintf(stderr,"Failed to create shared memory!"); 
		exit(EXIT_FAILURE);
	}

    	sem_union.val = 1;
    	if (semctl(fmutex, 0, SETVAL, sem_union) == -1) {
		fprintf(stderr,"Failed to set semaphore!"); 
		exit(EXIT_FAILURE);
	}

    	sem_union.val = 0;
    	if (semctl(rdcntmutex, 0, SETVAL, sem_union) == -1) {
		fprintf(stderr,"Failed to set semaphore!"); 
		exit(EXIT_FAILURE);
	}

    // 	sem_union.val = BUFFER_SIZE;
    // 	if (semctl(empty, 0, SETVAL, sem_union) == -1) {
	// 	fprintf(stderr,"Failed to set semaphore!"); 
	// 	exit(EXIT_FAILURE);
	// }

	if ( (shared_memory = shmat(shmid,(void *)0,0) ) == (void *)-1) {
		fprintf(stderr,"shmat failed\n");
		exit(EXIT_FAILURE);
	}
	shared_stuff = (struct shared_use_st *)shared_memory;

	for(i=0;i<BUFFER_SIZE;i++)
	{
		shared_stuff->buffer[i] = 0;
	}
	shared_stuff -> lo = 0;
	shared_stuff -> hi = 0;
	shared_stuff -> cur = 0;

  	exit(EXIT_SUCCESS);
}
```



修改makefile，使其能编译运行reader和writer：

```makefile
default: myipc init.c producer.c consumer.c
	gcc -o init myipc.o init.c
	gcc -o producer myipc.o producer.c
	gcc -o consumer myipc.o consumer.c
	gcc -o reader myipc.o reader.c
	gcc -o writer myipc.o writer.c
myipc: myipc.c
	gcc -c myipc.c
```



###四、测试结果

进入文件目录，编译运行

```shell
cd /home/yuqianhou/文档/pc_semaphore
make
./init
```

先运行读者：

```shell
./reader
```

再新建一个shell，运行写者：

```shell
./writer
```

运行结果如下：

![屏幕快照 2019-11-02 10.50.44](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/os_code/ipc/pc_semaphore/截图/屏幕快照 2019-11-02 10.50.44.png)

![屏幕快照 2019-11-02 10.50.56](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/os_code/ipc/pc_semaphore/截图/屏幕快照 2019-11-02 10.50.56.png)

![屏幕快照 2019-11-02 10.52.10](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/os_code/ipc/pc_semaphore/截图/屏幕快照 2019-11-02 10.52.10.png)

![屏幕快照 2019-11-02 10.52.21](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/os_code/ipc/pc_semaphore/截图/屏幕快照 2019-11-02 10.52.21.png)



### 五、遇到的问题和解决办法

####5.1 编译代码问题

最开始编译示例代码时，由于对文件及命令不熟悉，且`README中`提到：

```markdown
type "make" to compile

use "init" each time to cleanup 
```

因此我误以为直接输入`make`和`init`两条命令即可运行，导致无法运行出结果：

![屏幕快照 2019-11-02 10.52.55](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/os_code/ipc/pc_semaphore/截图/屏幕快照 2019-11-02 10.52.55.png)



#### 5.2 修改init文件

编写好代码后，放入Ubuntu中运行时，发现输入`./reader`命令后并没有出现结果。于是发现init文件之前是用来初始化`producer`和`consumer`的。最后将其修改成用来初始化`reader`和`writer`即可。

