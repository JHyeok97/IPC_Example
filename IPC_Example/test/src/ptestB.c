#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "testB.h"

#define PIPENAME1 "./named_PIPE1"
#define PIPENAME2 "./named_PIPE2"

static void Snd_Data(int fd, int count);
static int Rcv_Data(int fd, int count);

void *func2(void *arg)
{
    int fd_a, fd_b;
    int count = 0;

    if (access(PIPENAME1, F_OK) != 0)
    {
        mkfifo(PIPENAME1, 0666);
    }
    fd_a = open(PIPENAME1, O_RDONLY);
    if (fd_a < 0)
    {
        perror("open");
        exit(1);
    }

    if (access(PIPENAME2, F_OK) != 0)
    {
        mkfifo(PIPENAME2, 0666);
    }

    fd_b = open(PIPENAME2, O_WRONLY);
    if (fd_b < 0)
    {
        perror("open");
        exit(1);
    }

    while (1)
    {
        /* pipe A에서 count 값을 읽기 */
        read(fd_a, &count, sizeof(count));
        printf("Thread B: count = %d\n", count);
        count++;
        /* count 값을 pipe B에 쓰기 */
        write(fd_b, &count, sizeof(count));
        sleep(1);
    }
}

/*static void Snd_Data(int fd, int count)
{
    int nread;

    nread = write(fd, &count, sizeof(count));
    if (nread < 0)
    {
        perror("fail to write named pipe_B\n");
    }
}

static int Rcv_Data(int fd, int count)
{
    int nread;

    nread = read(fd, &count, sizeof(count));
    if (nread < 0)
    {
        perror("fail to read named pipe_B\n");
    }

    return count;
}
*/