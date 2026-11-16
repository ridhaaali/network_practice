#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 7000
#define MAX 1024

int main()
{
    srand(time(NULL));
    int sockfd;
    struct sockaddr_in address;
    socklen_t addrlen=sizeof(address);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    address.sin_port=htons(PORT);
    connect(sockfd,(struct sockaddr *)&address,addrlen);
    while(1)
    {
        int choice,n;
        printf("Enter 1 for user input\n");
        printf("Enter 2 for random\n");
        printf("Enter 999 to exit\n");
        scanf("%d",&choice);
        if (choice==999)
        {
            n=999;
            send(sockfd,&n,sizeof(n),0);
            break;
        }
        printf("Enter order of matrix:");
        scanf("%d",&n);
        int matrix[n][n];
        for(int i=0;i<n;i++)
        {
            for (int j=0;j<n;j++)
            {
                if(choice==1)
                {
                    scanf("%d",&matrix[i][j]);
                }
                else if(choice==2)
                {
                    matrix[i][j]=rand()%10;
                }
            }
        }
        send(sockfd,&n,sizeof(n),0);
        send(sockfd,matrix,sizeof(matrix),0);
        char result[1024];
        int m=read(sockfd,result,sizeof(result));
        result[m]='\0';
        printf("%s\n\n",result);
    }
    close(sockfd);
    return 0;
}