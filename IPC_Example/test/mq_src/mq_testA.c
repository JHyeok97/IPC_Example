#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#include "testA.h"

void *func1(void *arg)
{
    struct message msg;
    key_t key1 = 1234, key2 = 4321;
    int mqID1, mqID2;
    msg.count = 0;

    // mqID 얻어옴
    if ((mqID1 = msgget(key1, IPC_CREAT | 0666)) == -1)
    {
        perror("snd msgget failed_A\n");
    }
    if ((mqID2 = msgget(key2, IPC_CREAT | 0666)) == -1)
    {
        perror("snd msgget failed_A\n");
    }

    while (msg.count < 100)
    {
        msg.count++;
        if (msgsnd(mqID1, &msg, sizeof(struct message), 0) < 0)
            printf("nonononoA\n");

        msgrcv(mqID2, &msg, sizeof(struct message), 0, 0);
        printf("thread A: %d\n", msg.count);
    }

    msgctl(mqID1, IPC_RMID, NULL);
    msgctl(mqID2, IPC_RMID, NULL);
}
