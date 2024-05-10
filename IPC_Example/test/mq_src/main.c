#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <regex.h>
#include <fcntl.h>

#include "testA.h"
#include "testB.h"

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