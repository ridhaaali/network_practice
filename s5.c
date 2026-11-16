#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 7000
#define MAX 1024

int main()
{
    int sockfd,new_socket;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    char msg[MAX],buffer[MAX];

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);
    bind(sockfd,(struct sockaddr *)&address,addrlen);
    listen(sockfd,3);
    printf("Server is listening on port %d\n",PORT);
    new_socket=accept(sockfd,(struct sockaddr *)&address,&addrlen);
    while(1)
    {
        int n=read(new_socket,buffer,sizeof(buffer)-1);
        buffer[n]='\0';
        printf("Client:%s\n",buffer);
        if(strcmp(buffer,"exit")==0 || strcmp(buffer,"exit\n")==0)
        {
            printf("CLient Disconnected");
            break;
        }
        printf("Enter message for client:");
        fgets(msg,sizeof(msg),stdin);
        send(new_socket,msg,strlen(msg),0);
        if(strcmp(msg,"exit")==0 || strcmp(msg,"exit\n")==0)
        {
            printf("Server Disconnected");
            break;
        }
    }
    close(new_socket);
    close(sockfd);
    return 0;
}