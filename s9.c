#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 7000
#define MAX 1024
int main()
{
    int sockfd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    char filename[MAX],msg[3000];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);
    bind(sockfd,(struct sockaddr *)&address,addrlen);
    listen(sockfd,3);
    printf("Server is listening on port %d\n",PORT);
    while(1)
    {
        new_socket=accept(sockfd,(struct sockaddr *)&address, &addrlen);
        if(fork()==0)
        {
            int n=read(new_socket,filename,sizeof(filename));
            filename[n]='\0';
            printf("Client Requested:%s\n",filename);
            pid_t pid=getpid();
            FILE *fp=fopen(filename,"r");
            if(fp==NULL)
            {
                snprintf(msg,sizeof(msg),"Process id %d could not find file %s",pid,filename);
                send(new_socket,msg,strlen(msg),0);
            }
            else
            {
                snprintf(msg,sizeof(msg),"Process id %d sending find file %s",pid,filename);
                send(new_socket,msg,strlen(msg),0);
                while(fgets(msg,sizeof(msg),fp)!=NULL)
                {
                    send(new_socket,msg,strlen(msg),0);
                }
                fclose(fp);
            }
            close(new_socket);
            exit(0);
        }
        else
        {
            close(new_socket);
        }
    }
    close(sockfd);
    return 0;
}