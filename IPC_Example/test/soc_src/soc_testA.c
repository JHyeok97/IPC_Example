#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define FILESERVER "./sockpath"

void *func1(void *arg)
{
    int server_sockfd, client_sockfd;
    int client_addr_size;
    int count = 0;
    int option;

    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;

    if (access(FILESERVER, F_OK) == 0)
    {
        unlink(FILESERVER);
    }

    server_sockfd = socket(PF_FILE, SOCK_STREAM, 0);
    if (server_sockfd < 0)
    {
        perror("server fd error_A \n");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, FILESERVER);

    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        perror("bind_A error\n");
    }

    while (1)
    {
        if (listen(server_sockfd, 5) < 0)
        {
            perror("대기 상태 모드 설정 실패");
        }

        client_addr_size = sizeof(client_addr);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_addr_size);

        if (client_sockfd < 0)
        {
            perror("클라이언트 연결 수락 실패\n");
        }

        while (count < 100)
        {
            read(client_sockfd, &count, sizeof(count));
            printf("thread A: %d\n", count);

            count++;
            write(client_sockfd, &count, sizeof(count));
        }

        printf("AAAAA\n");
        close(client_sockfd);
        break;
    }
}
