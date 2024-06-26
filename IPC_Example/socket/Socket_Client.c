#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
    int client_len;
    int client_sockfd;

    FILE *fp_in;
    char buf_in[255];
    char buf_get[255];

    struct sockaddr_un clientaddr;

    if (argc != 2)
    {
        printf("Usage : ./zipcode_cl [file_name]\n");
        printf("예 : ./zipcode_cl /tmp/mysocket\n");
        exit(0);
    }

    client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sockfd == -1)

    {
        perror("error : ");
        exit(0);
    }
    bzero(&clientaddr, sizeof(clientaddr));
    clientaddr.sun_family = AF_UNIX;
    strcpy(clientaddr.sun_path, argv[1]);
    client_len = sizeof(clientaddr);
    if (connect(client_sockfd,
                (struct sockaddr *)&clientaddr, client_len) < 0)

    {
        perror("Connect error: ");
        exit(0);
    }
    while (1)

    {
        printf("지역이름 입력 : ");
        fgets(buf_in, 255, stdin);

        buf_in[strlen(buf_in) - 1] = '0';
        write(client_sockfd, buf_in, 255);
        if (strncmp(buf_in, "quit", 4) == 0)
        {
            close(client_sockfd);
            exit(0);
        }
        while (1)

        {
            read(client_sockfd, buf_get, 255);
            if (strncmp(buf_get, "end", 3) == 0)
                break;

            printf("%s", buf_get);
        }
    }

    close(client_sockfd);
    exit(0);
}