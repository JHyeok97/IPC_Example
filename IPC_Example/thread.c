#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;

int var1 = 1;

void disp(char *n)
{
    sem_wait(&semaphore);
    int temp = var1;
    printf("Thread %s : %d\n", n, temp++);
    var1 = temp;
    sem_post(&semaphore);
}

void *func1(void *arg)
{
    while (var1 <= 100)
    {
        disp("A");
        usleep(1);
    }

    pthread_exit(NULL);
}

void *func2(void *arg)
{
    while (var1 <= 100)
    {
        disp("B");
        usleep(1);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid1, tid2;
    int id1, id2;

    sem_init(&semaphore, 0, 1);

    id1 = pthread_create(&tid1, NULL, func1, NULL);
    if (id1 != 0)
    {
        perror("thread create error\n");
        exit(1);
    }

    id2 = pthread_create(&tid2, NULL, func2, NULL);
    if (id2 != 0)
    {
        perror("thread create error\n");
        exit(1);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&semaphore);

    return 0;
}