#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 7000
#define SIZE 1024
int main()
{
    int sockfd, n;
    struct sockaddr_in server_address;
    socklen_t addrlen = sizeof(server_address);
    char sentence[SIZE], result[SIZE];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    while (1)
    {
        printf("Enter a new generation sentence (exit to quit): ");
        fgets(sentence, SIZE, stdin);
        sentence[strcspn(sentence, "\n")] = '\0';
        sendto(sockfd, sentence, strlen(sentence) + 1, 0,(struct sockaddr *)&server_address, addrlen);
        if(strcmp(sentence, "exit") == 0)
        {
            printf("Client disconnected.\n");
            break;
        }
        n = recvfrom(sockfd, result, SIZE - 1, 0, NULL, NULL);
        result[n] = '\0';
        printf("Translated sentence: %s\n\n", result);
    }
    close(sockfd);
    return 0;
}
