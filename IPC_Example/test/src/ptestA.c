#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "testA.h"

#define PIPENAME1 "./named_PIPE1"
#define PIPENAME2 "./named_PIPE2"

static void Snd_Data(int fd, int count);
static int Rcv_Data(int fd, int count);

void *func1(void *arg)
{
    int fd_a, fd_b;
    int count = 0;

    if (access(PIPENAME1, F_OK) != 0)
    {
        mkfifo(PIPENAME1, 0666);
    }
    fd_a = open(PIPENAME1, O_WRONLY);
    if (fd_a < 0)
    {
        perror("open");
        exit(1);
    }

    if (access(PIPENAME2, F_OK) != 0)
    {
        mkfifo(PIPENAME2, 0666);
    }

    fd_b = open(PIPENAME2, O_RDONLY);
    if (fd_b < 0)
    {
        perror("open");
        exit(1);
    }

    while (1)
    {
        count++;

        /* count 값을 pipe A에 쓰기 */
        write(fd_a, &count, sizeof(count));

        /* pipe B에서 count 값을 읽기 */
        read(fd_b, &count, sizeof(count));
        printf("Thread A: count = %d\n", count);
        sleep(1);
    }
}
