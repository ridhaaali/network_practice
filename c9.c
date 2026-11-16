#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 7000
#define MAX 1024
int main()
{
    int sockfd;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    char filename[MAX],buff[MAX];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    address.sin_port=htons(PORT);
    connect(sockfd,(struct sockaddr *)&address,addrlen);
    printf("Enter filename:");
    fgets(filename,sizeof(filename),stdin);
    filename[strcspn(filename,"\n")]='\0';
    send(sockfd,filename,strlen(filename),0);
    int n;
    while((n=read(sockfd,buff,sizeof(buff)))>0)
    {
        buff[n]='\0';
        printf("%s",buff);
    }
    close(sockfd);
    return 0;
}