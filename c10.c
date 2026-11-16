#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define PORT 7000
#define SIZE 1024
int main()
{
    int sock, n;
    char buffer[SIZE];
    struct sockaddr_in address;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sock, (struct sockaddr *)&address, sizeof(address)); 
    printf("Connected to server!\n");
    n = recv(sock, buffer, SIZE - 1, 0);
    buffer[n] = '\0';
    printf("%s", buffer);
    if(fork() == 0)
    {
        while((n = recv(sock, buffer, SIZE - 1, 0)) > 0)
        {
            buffer[n] = '\0';
            printf("%s", buffer);
            fflush(stdout);
        }
        close(sock);
        exit(0);
    }
    while(fgets(buffer, SIZE, stdin))
    {
        send(sock, buffer, strlen(buffer), 0);
        if(strncmp(buffer, "exit", 4) == 0)
            break;
    }
    shutdown(sock, SHUT_WR);
    wait(NULL);
    close(sock);
    return 0;
}
