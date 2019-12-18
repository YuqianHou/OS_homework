// #include "stdio.h"
// #include <stdlib.h>
// #include <pthread.h>
// #include <semaphore.h>

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
	
	// for(i=0;i<30;i++){
	// 	sem_p(full);
	// 	sem_p(mutex);
	// 	//item = remove_item();
	// 	item = shared_stuff->buffer[shared_stuff->lo];
	// 	(shared_stuff->buffer)[(shared_stuff->lo)]=0;
	// 	(shared_stuff->lo) = ((shared_stuff->lo)+1) % BUFFER_SIZE;
	// 	printf("Removing item %d\n",item);
	// 	//display_buffer();
	// 	sem_v(mutex);
	// 	sem_v(empty);
	// 	//consume_item(item);
	// 	printf("Consuming item %d\n",item);
	// 	sleep(2);
	// }

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

// #define N_WRITER 30 //写者数目
// #define N_READER 5 //读者数目
// #define W_SLEEP  1 //控制写频率
// #define R_SLEEP  1 //控制读频率


// pthread_t wid[N_WRITER],rid[N_READER];
// pthread_mutex_t mutex_write;//同一时间只能一个人写文件,互斥
// sem_t sem_read;//同一时间只能有一个人访问 readerCnt
// int data = 0;
// int readerCnt = 0;
// void write()
// {
//     int rd = rand();
//     printf("write %d\n",rd);
//     data = rd;
// }
// void read()
// {
//     printf("read %d\n",data);
// }
// void * writer(void * in)
// {
// //    while(1)
// //    {
//         pthread_mutex_lock(&mutex_write);
//         printf("写线程id%d进入数据集\n",pthread_self());
//         write();
//         printf("写线程id%d退出数据集\n",pthread_self());
//         pthread_mutex_unlock(&mutex_write);
//         sleep(W_SLEEP);
// //    }
//     pthread_exit((void *) 0);
// }

// void * reader (void * in)
// {
// //    while(1)
// //    {
//         sem_wait(&sem_read);
//         readerCnt++;
//         if(readerCnt == 1){
//             pthread_mutex_lock(&mutex_write);
//         }
//         sem_post(&sem_read);
//         printf("读线程id%d进入数据集\n",pthread_self());
//         read();
//         printf("读线程id%d退出数据集\n",pthread_self());    
//         sem_wait(&sem_read);
//         readerCnt--;
//         if(readerCnt == 0){
//             pthread_mutex_unlock(&mutex_write);
//         }
//         sem_post(&sem_read);
//         sleep(R_SLEEP);
// //    }
//     pthread_exit((void *) 0);
// }

// int main()
// {
//     printf("多线程,读者优先\n");    
//     pthread_mutex_init(&mutex_write,NULL);
//     sem_init(&sem_read,0,1);
//     int i = 0;
//     for(i = 0; i < N_WRITER; i++)
//     {
//         pthread_create(&wid[i],NULL,writer,NULL);
//     }
//         for(i = 0; i < N_READER; i++)
//     {
//         pthread_create(&rid[i],NULL,reader,NULL);
//     }
//     sleep(1);
//     return 0;
// }