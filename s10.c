#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define PORT 7000
#define MAX_CLIENTS 10
#define SIZE 1024
int main()
{
    int server_fd, new_socket;
    int clients[MAX_CLIENTS] = {0};
    int ids[MAX_CLIENTS] = {0};
    int next_id = 1;
    struct sockaddr_in address;
    fd_set readfds;
    char buffer[SIZE], message[SIZE + 50];
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);
    printf("Server listening...\n");
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        int maxfd = server_fd;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
            if(clients[i] > 0)
            {
                FD_SET(clients[i], &readfds);
                if(clients[i] > maxfd)
                    maxfd = clients[i];
            }
        }
        select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(server_fd, &readfds))
        {
            new_socket = accept(server_fd, NULL, NULL);
            if(new_socket >= 0)
            {
                int i;
                for(i = 0; i < MAX_CLIENTS; i++)
                {
                    if(clients[i] == 0)
                    {
                        clients[i] = new_socket;
                        ids[i] = next_id++;
                        printf("Client %d connected\n", ids[i]);
                        snprintf(message, sizeof(message), "Welcome! You are Client %d\n", ids[i]);
                        send(new_socket, message, strlen(message), 0);
                        break;
                    }
                }
                if(i == MAX_CLIENTS)
                {
                    char msg[] = "Server is full!\n";
                    send(new_socket, msg, strlen(msg), 0);
                    close(new_socket);
                }
            }
        }
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
            if(clients[i] > 0 && FD_ISSET(clients[i], &readfds))
            {
                int n = read(clients[i], buffer, SIZE - 1);
                if(n <= 0)
                {
                    printf("Client %d disconnected\n", ids[i]);
                    close(clients[i]);
                    clients[i] = 0;
                    ids[i] = 0;
                }
                else
                {
                    buffer[n] = '\0';
                    printf("Client %d: %s", ids[i], buffer);
                    snprintf(message, sizeof(message), "Client %d: %s", ids[i], buffer);
                    for(int j = 0; j < MAX_CLIENTS; j++)
                    {
                        if(clients[j] > 0 && clients[j] != clients[i])
                        {
                            send(clients[j], message, strlen(message), 0);
                        }
                    }
                }
            }
        }
    }
    close(server_fd);
    return 0;
}
