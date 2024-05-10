#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "testB.h"

#define MSG_SIZE 80
#define PIPENAME "./named_PIPE"

static void send_data(int fd, int *msg);
static void rcv_data(int fd, int *msg);

int var;

void *func2(void *arg)
{
    int fd;
    int msg[MSG_SIZE];
    int ac, rc;

    // if exist named file, delete it
    ac = access(PIPENAME, F_OK);
    if (ac == 0)
    {
        unlink(PIPENAME);
    }

    // create file
    rc = mkfifo(PIPENAME, 0666);
    if (rc < 0)
    {
        perror("fail to make named pipe\n");
        exit(1);
    }

    fd = open(PIPENAME, O_RDWR);
    if (fd < 0)
    {
        perror("fail to open named pipe\n");
        exit(1);
    }

    while (1)
    {
        rcv_data(fd, msg);
        printf("thread B : %d\n", var);

        usleep(100);
        send_data(fd, msg);
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

static void rcv_data(int fd, int *msg)
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