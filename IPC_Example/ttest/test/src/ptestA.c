#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "testA.h"

#define MSG_SIZE 80
#define PIPENAME "./named_PIPE"

static void send_data(int fd, int *msg);
static void recv_data(int fd, int *msg);

static int var = 1;
void *func1(void *arg)
{
    int msg[MSG_SIZE];
    int fd;

    // file open
    fd = open(PIPENAME, O_CREAT | O_RDWR);
    if (fd < 0)
    {
        perror("fail to open named pipe\n");
        exit(1);
    }

    while (1)
    {
        printf("thread A : %d\n", var);
        send_data(fd, msg);
        usleep(100);
        recv_data(fd, msg);
        if (var >= 100)
        {
            break;
        }
    }

    pthread_exit(NULL);
}

static void send_data(int fd, int *msg)
{
    int nread;
    msg[0] = var;
    nread = write(fd, msg, sizeof(msg));
    if (nread < 0)
    {
        perror("fail to call write()\n");
        exit(1);
    }
}

static void recv_data(int fd, int *msg)
{
    int nread;
    nread = read(fd, msg, sizeof(msg));
    if (nread < 0)
    {
        perror("fail to call read()\n");
        exit(1);
    }
    var = (*msg) + 1;
}
