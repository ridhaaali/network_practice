#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 7000
#define SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    char msg[SIZE],buffer[SIZE],w[100];
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);
    bind(sockfd,(struct sockaddr *)&address,addrlen);
    printf("UDP Translation server started\n");
    while(1)
    {
        int n=recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&address,&addrlen);
        buffer[n]='\0';
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Client disconnected.\n");
            break;
        }
        printf("Received: %s\n",buffer);
        msg[0]='\0';
        char *p=strtok(buffer," \n");
        while(p!=NULL)
        {
            char punct[10];
            int len=strlen(p),i=len;
            while(i>0 && strchr(",.!?;",p[i-1]))
            {
                i--;
            }
            strcpy(punct,p+i);
            strncpy(w,p,i);
            w[i]='\0';
            if (!strcmp(w, "tbh")) strcpy(w, "to be honet");
            else if (!strcmp(w, "ig")) strcpy(w, "i guess");
            else if (!strcmp(w, "tbf")) strcpy(w, "to be fair");
            else if (!strcmp(w, "atm")) strcpy(w, "at the moment");
            else if (!strcmp(w, "irl")) strcpy(w, "in real life");
            else if (!strcmp(w, "lol")) strcpy(w, "laugh out loud");
            else if (!strcmp(w, "asap")) strcpy(w, "as soon as possible");
            else if (!strcmp(w, "omg")) strcpy(w, "oh my god");
            else if (!strcmp(w, "ttyl")) strcpy(w, "talk to you later");
            else if (!strcmp(w, "idk")) strcpy(w, "i dont know");
            else if (!strcmp(w, "nvm")) strcpy(w, "Nevermind");
            else if (!strcmp(w, "idc")) strcpy(w, "i dont care");
            strcat(w,punct);
            if(msg[0])
            {
                strcat(msg," ");
            }
            strcat(msg,w);
            p=strtok(NULL," \n");
        }
        printf("Translated: %s\n", msg);
        sendto(sockfd, msg, strlen(msg) + 1, 0,(struct sockaddr *)&address, addrlen);
    }
    close(sockfd);
    return 0;
}
