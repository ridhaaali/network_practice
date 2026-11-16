#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <arpa/inet.h>
#include <signal.h>
#include<string.h>
#define PORT 7000
#define SIZE 1024

void handle_client(int sockfd, struct sockaddr_in address, socklen_t addrlen)
{
    time_t current_time;
    char * time_string;
    current_time=time(NULL);
    time_string=ctime(&current_time);
    sendto(sockfd,time_string,strlen(time_string),0,(struct sockaddr *)&address,addrlen);
    printf("TIme sent:%s",time_string);
    close(sockfd);
    exit(0);
}
int main()
{
    int sockfd;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    char buff[SIZE];
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    address.sin_family=AF_INET;
    address.sin_port=htons(PORT);
    address.sin_addr.s_addr=INADDR_ANY;
    signal(SIGCHLD,SIG_IGN);
    bind(sockfd,(struct sockaddr *)&address,addrlen);
    while(1)
    {
        int n=recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&address,&addrlen);
        buff[n]='\0';
        if(fork()==0)
        {
            handle_client(sockfd,address,addrlen);
        }
    }
    close(sockfd);
    return(0);
}