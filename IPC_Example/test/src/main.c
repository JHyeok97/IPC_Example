#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "testA.h"
#include "testB.h"

#define PIPENAME1 "./named_PIPE1"
#define PIPENAME2 "./named_PIPE2"

int main(void)
{
    pthread_t tid1, tid2;
    int id1, id2;

    id1 = pthread_create(&tid1, NULL, func1, NULL);
    if (id1 != 0)
    {
        perror("thread create fail");
        exit(1);
    }

    id2 = pthread_create(&tid2, NULL, func2, NULL);
    if (id2 != 0)
    {
        perror("thread create fail");
        exit(1);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}