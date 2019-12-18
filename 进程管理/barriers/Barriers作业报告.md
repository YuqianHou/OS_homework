# Barriers作业报告

<center>软工2班 3017218092 侯雨茜</center>



###一、准备工作

下载[barrier.c](https://pdos.csail.mit.edu/6.828/2018/homework/barrier.c)并在电脑上进行编译：

![屏幕快照 2019-12-01 19.58.12](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/barriers/截图/屏幕快照 2019-12-01 19.58.12.png)

2指定在barrier上同步的线程数（ `barrier.c` 中的`nthread`）。每个线程都处于紧密的循环中。在每个循环迭代中，线程都会调用`barrier()`，然后在随机的微秒数内休眠。断言触发，因为一个线程在另一线程到达屏障之前就离开了屏障。理想的行为是所有线程都应阻塞，直到`nthread`调用 `barrier`为止。

目标是实现所需的行为。除了以前看到的[锁原语](https://pdos.csail.mit.edu/6.828/2018/homework/lock.html)之外，您还将需要以下新的pthread原语：

```c
pthread_cond_wait(&cond, &mutex);  // go to sleep on cond, releasing lock mutex
pthread_cond_broadcast(&cond);     // wake up every thread sleeping on cond
pthread_cond_wait releases the mutex when called, and re-acquires the mutex before returning.
```



### 二、实现barrier()

```c
static void 
barrier()
{
    pthread_mutex_lock(&bstate.barrier_mutex);
    bstate.nthread++;
    printf("in round %d as %d\n", bstate.round, bstate.nthread);
    if (bstate.nthread != nthread)
    {
        pthread_cond_wait(&bstate.barrier_cond, &bstate.barrier_mutex);
    }
    else
    {
        bstate.round++;
        bstate.nthread = 0;
        pthread_cond_broadcast(&bstate.barrier_cond);
    }
    pthread_mutex_unlock(&bstate.barrier_mutex);
}
```



### 三、测试结果

使用一个、两个和多个线程测试代码。

#### 1个线程

![屏幕快照 2019-12-01 20.47.36](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/barriers/截图/屏幕快照 2019-12-01 20.47.36.png)

……

![屏幕快照 2019-12-01 20.48.12](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/barriers/截图/屏幕快照 2019-12-01 20.48.12.png)



####2个线程

![屏幕快照 2019-12-01 20.48.24](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/barriers/截图/屏幕快照 2019-12-01 20.48.24.png)

……

![屏幕快照 2019-12-01 20.48.48](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/barriers/截图/屏幕快照 2019-12-01 20.48.48.png)



#### 3个线程

![屏幕快照 2019-12-01 20.48.58](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/barriers/截图/屏幕快照 2019-12-01 20.48.58.png)

……

![屏幕快照 2019-12-01 20.50.33](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/进程管理/barriers/截图/屏幕快照 2019-12-01 20.50.33.png)


