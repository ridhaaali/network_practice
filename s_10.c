#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define PORT 7000
#define MAX_CLIENTS 10
#define SIZE 1024
int main()
{
    int sockfd,new_socket;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    int clients[MAX_CLIENTS]={0};
    int ids[MAX_CLIENTS]={0};
    int next_id=1;
    fd_set readfds;
    char buffer[SIZE],msg[SIZE+50];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);
    bind(sockfd,(struct sockaddr *)&address,addrlen);
    listen(sockfd,5);
    printf("Server Listening on port %d\n",PORT);
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(sockfd,&readfds);
        int max_fd=sockfd;
        for (int i=0;i<MAX_CLIENTS;i++)
        {
            if(clients[i]>0)
            {
                FD_SET(clients[i],&readfds);
                if(clients[i]>max_fd)
                {
                    max_fd=clients[i];
                }
            }
        }
        select(max_fd+1,&readfds,NULL,NULL,NULL);
        if(FD_ISSET(sockfd,&readfds))
        {
            new_socket=accept(sockfd,NULL,NULL);
            if(new_socket>=0)
            {
                int i;
                for(i=0;i<MAX_CLIENTS;i++)
                {
                    if(clients[i]==0)
                    {
                        clients[i]=new_socket;
                        ids[i]=next_id++;
                        printf("Client %d Connected\n",ids[i]);
                        snprintf(msg,sizeof(msg),"Welcome Client %d\n",ids[i]);
                        send(new_socket,msg,strlen(msg),0);
                        break;
                    }
                }
                if (i==MAX_CLIENTS)
                {
                    char msg[]="Server is Full\n";
                    send(new_socket,msg,strlen(msg),0);
                    close(new_socket);
                }
            }
        }
        for(int i=0;i<MAX_CLIENTS;i++)
        {
            if(clients[i]>0 && FD_ISSET(clients[i],&readfds))
            {
                int n=read(clients[i],buffer,SIZE-1);
                if(n<=0)
                {
                    close(clients[i]);
                    clients[i]=0;
                    ids[i]=0;
                }
                else
                {
                    buffer[n]='\0';
                    printf("Client %d:%s\n",ids[i],buffer);
                    snprintf(msg,sizeof(msg),"Client %d:%s",ids[i],buffer);
                    for(int j=0;j<MAX_CLIENTS;j++)
                    {
                        if(clients[j]>0 && clients[j]!=clients[i])
                        {
    
                            send(clients[j],msg,strlen(msg),0);
                        }
                    }
                }
            }
        }
    }  
    close(sockfd);
    return 0;
}