#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 7000
#define MAX 1024

int main()
{
    int sockfd,new_socket;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);
    bind(sockfd,(struct sockaddr *)&address,addrlen);
    listen(sockfd,3);
    printf("Server Listening on Port %d\n",PORT);
    new_socket=accept(sockfd,(struct sockaddr *)&address,&addrlen);
    while(1)
    {
        int n;
        read(new_socket,&n,sizeof(n));
        if(n==999)
        {
            break;
        }
        printf("order of matrix:%d\n",n);
        int matrix[n][n];
        read(new_socket,matrix,sizeof(matrix));
        printf("Matrix:\n");
        for(int i=0;i<n;i++)
        {
            for (int j=0;j<n;j++)
            {
                printf("%d ",matrix[i][j]);
            }
            printf("\n");
        }
        int upper=1,diagonal=1,lower=1;
        for(int i=0;i<n;i++)
        {
            for (int j=0;j<n;j++)
            {
                if(i!=j && matrix[i][j]!=0)
                {
                    diagonal=0;
                }
                if(i>j && matrix[i][j]!=0)
                {
                    upper=0;
                }
                if(i<j && matrix[i][j]!=0)
                {
                    lower=0;
                }
            }
        }
        char *result;
        if(diagonal)
        {
            result="diagonal";
        }
        else if(upper)
        {
            result="upper";
        }
        else if(lower)
        {
            result="lower";
        }
        else
        {
            result="not triangular";
        }
        send(new_socket,result,strlen(result),0);
    }
    close(new_socket);
    close(sockfd);
    return 0;
}