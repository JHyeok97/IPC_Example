#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PIPE_NAME_A "./pipe_a"
#define PIPE_NAME_B "./pipe_b"

void *threadA(void *arg)
{
    int fd_a, fd_b;
    int count = 0;

    fd_a = open(PIPE_NAME_A, O_WRONLY);
    if (fd_a < 0)
    {
        perror("open");
        exit(1);
    }

    fd_b = open(PIPE_NAME_B, O_RDONLY);
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

    return NULL;
}

void *threadB(void *arg)
{
    int fd_a, fd_b;
    int count = 0;

    fd_a = open(PIPE_NAME_A, O_RDONLY);
    if (fd_a < 0)
    {
        perror("open");
        exit(1);
    }

    fd_b = open(PIPE_NAME_B, O_WRONLY);
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

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread_a, thread_b;

    /* named pipe 생성 */
    mkfifo(PIPE_NAME_A, 0666);
    mkfifo(PIPE_NAME_B, 0666);

    /* Thread A 생성 */
    pthread_create(&thread_a, NULL, (void *)threadA, NULL);
    /* Thread B 생성 */
    pthread_create(&thread_b, NULL, (void *)threadB, NULL);

    /* Thread A와 Thread B 종료 대기 */
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    /* named pipe 삭제 */
    unlink(PIPE_NAME_A);
    unlink(PIPE_NAME_B);

    return 0;
}