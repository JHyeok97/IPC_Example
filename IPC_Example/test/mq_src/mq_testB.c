#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#include "testA.h"

void *func2(void *arg)
{
    key_t key1 = 1234, key2 = 4321;
    struct message msg;
    int mqID1, mqID2;
    msg.count = 0;

    // 받아오는 mqID 얻어오기
    if ((mqID1 = msgget(key1, IPC_CREAT | 0666)) == -1)
    {
        perror("rcv msgget failed_B\n");
    }

    if ((mqID2 = msgget(key2, IPC_CREAT | 0666)) == -1)
    {
        perror("rcv msgget failed_B\n");
    }

    while (msg.count < 100)
    {
        if (msgrcv(mqID1, &msg, sizeof(struct message), 0, 0) < 0)
            printf("nonono_B\n");
        printf("thread B: %d\n", msg.count);

        msg.count++;
        msgsnd(mqID2, &msg, sizeof(struct message), 0);
    }
    msgctl(mqID1, IPC_RMID, NULL);
    msgctl(mqID2, IPC_RMID, NULL);
}