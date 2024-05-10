#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int server_sockfd, client_sockfd;
    int state, client_len;
    pid_t pid;

    FILE *fp;
    struct sockaddr_un clientaddr, serveraddr;

    char buf[255];
    char line[255];

    if (argc != 2)
    {
        printf("Usage : ./zipcode [file_name]\n");
        printf("예    : ./zipcode /tmp/mysocket\n");
        exit(0);
    }

    memset(line, '0', 255);
    state = 0;

    if (access(argv[1], F_OK) == 0)
    {
        unlink(argv[1]);
    }
    // 주소 파일을 읽어들인다.
    client_len = sizeof(clientaddr);
    if ((fp = fopen("zipcode.txt", "r")) == NULL)
    {
        perror("file open error : ");
        exit(0);
    }

    // internet 기반의 스트림 소켓을 만들도록 한다.
    if ((server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error : ");
        exit(0);
    }
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, argv[1]);

    state = bind(server_sockfd, (struct sockaddr *)&serveraddr,
                 sizeof(serveraddr));
    if (state == -1)
    {
        perror("bind error : ");
        exit(0);
    }

    state = listen(server_sockfd, 5);
    if (state == -1)
    {
        perror("listen error : ");
        exit(0);
    }

    printf("accept : \n");
    while (1)
    {
        client_sockfd = accept(server_sockfd,
                               (struct sockaddr *)&clientaddr, &client_len);
        printf("test test\n");
        pid = fork();
        if (pid == 0)
        {
            if (client_sockfd == -1)
            {
                perror("Accept error : ");
                exit(0);
            }
            while (1)
            {
                memset(buf, '0', 255);
                if (read(client_sockfd, buf, 255) <= 0)
                {
                    close(client_sockfd);
                    fclose(fp);
                    exit(0);
                }

                if (strncmp(buf, "quit", 4) == 0)
                {
                    write(client_sockfd, "bye bye\n", 8);
                    close(client_sockfd);
                    fclose(fp);
                    break;
                }

                while (fgets(line, 255, fp) != NULL)
                {
                    if (strstr(line, buf) != NULL)
                        write(client_sockfd, line, 255);
                    memset(line, '0', 255);
                }
                write(client_sockfd, "end", 255);
                printf("send end\n");
                rewind(fp);
            }
        }
    }
    close(client_sockfd);
}