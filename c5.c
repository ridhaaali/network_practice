#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 7000
#define MAX 1024

int main()
{
    int client_sockfd;
    struct sockaddr_in server_address;
    socklen_t addrlen=sizeof(server_address);
    char buffer[MAX],msg[1024];
    client_sockfd=socket(AF_INET,SOCK_STREAM,0);
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_address.sin_port=htons(PORT);
    connect(client_sockfd,(struct sockaddr *)&server_address,addrlen);
    while(1)
    {
        printf("Enter message for Server:");
        fgets(msg,sizeof(msg),stdin);
        send(client_sockfd,msg,strlen(msg),0);
        if(strcmp(msg,"exit")==0 || strcmp(msg,"exit\n")==0)
        {
            printf("Client Disconnected");
            break;
        }
        int n=read(client_sockfd,buffer,sizeof(buffer)-1);
        buffer[n]='\0';
        printf("Server:%s",buffer);
        if(strcmp(buffer,"exit")==0 || strcmp(buffer,"exit\n")==0)
        {
            printf("Server Disconnected");
            break;
        }
    }
    close(client_sockfd);
    return 0;
}