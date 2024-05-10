#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>

#define FILESERVER "./sockpath"

void *func2(void *arg)
{
    int client_sock;
    struct sockaddr_un server_addr;
    int count = 0;

    client_sock = socket(PF_FILE, SOCK_STREAM, 0);
    if (client_sock < 0)
    {
        perror("failed create socket_B\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, FILESERVER);
    while (1)
    {
        if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            perror("connect failed_B\n");
        }
        else
            break;
    }

    while (count < 100)
    {
        count++;
        write(client_sock, &count, sizeof(count));

        read(client_sock, &count, sizeof(count));
        printf("thread B: %d\n", count);
    }
    printf("BBBBB\n");
    close(client_sock);
}
