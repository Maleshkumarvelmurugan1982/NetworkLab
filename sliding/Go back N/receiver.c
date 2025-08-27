#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 2030
#define MAXSZ 100
#define MAX_FRAMES 30

typedef struct {
    int seq;
    char data[MAXSZ];
} Frame;

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[MAXSZ];
    int expectedSeq = 0;  // R

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket failed"); exit(1); }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed"); exit(1);
    }

    listen(sockfd, 5);
    printf("Receiver waiting for connection...\n");
    addr_size = sizeof(clientAddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);
    if (newsockfd < 0) { perror("Accept failed"); exit(1); }

    printf("Connected to sender: %s\n", inet_ntoa(clientAddr.sin_addr));

    int count = 0;
    while (count < MAX_FRAMES) {
        int n = recv(newsockfd, buffer, MAXSZ, 0);
        if (n <= 0) break;

        int recvSeq = buffer[0] - '0';
        printf("Received frame %d\n", recvSeq);

        if (recvSeq == expectedSeq) {
            printf("Frame %d accepted: %s\n", recvSeq, &buffer[1]);
            expectedSeq = (expectedSeq + 1) % 8; // 3-bit sequence
        } else {
            printf("Frame %d discarded, expecting %d\n", recvSeq, expectedSeq);
        }

        // Send cumulative ACK
        char ack = '0' + expectedSeq;
        send(newsockfd, &ack, 1, 0);
        printf("ACK %d sent\n", expectedSeq);
        count++;
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
