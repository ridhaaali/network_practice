#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 7000
#define SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    char buff[SIZE],msg[SIZE];
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    sendto(sockfd,msg,sizeof(msg),0,(struct sockaddr *)&address,addrlen);
    printf("Time request sent\n");
    int n=recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&address, &addrlen);
    buff[n]='\0';
    printf("Server:%s",buff);
    close(sockfd);
    return 0;
}