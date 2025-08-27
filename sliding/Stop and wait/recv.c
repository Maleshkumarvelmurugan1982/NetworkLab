#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 9876
#define MAXSZ 1024
#define LOSS_PROB 5  // 1 in 5 frames lost
#define CORRUPT_PROB 6 // 1 in 6 frames corrupted

typedef struct {
    int seq;         // sequence number 0 or 1
    int len;         // length of message
    char msg[MAXSZ]; // message
} Frame;

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    srand(time(NULL));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket"); exit(1); }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind"); exit(1);
    }

    listen(sockfd, 5);
    printf("Receiver waiting for connection...\n");

    newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &addrLen);
    if (newsockfd < 0) { perror("Accept"); exit(1); }

    printf("Connected to client %s\n", inet_ntoa(clientAddr.sin_addr));

    int expectedSeq = 0;
    Frame frame;
    int count = 0;

    while (count < 30) {
        int n = recv(newsockfd, &frame, sizeof(Frame), 0);
        if (n <= 0) { printf("Connection closed\n"); break; }

        // Simulate loss
        if (rand() % LOSS_PROB == 0) {
            printf("Frame %d lost!\n\n", frame.seq);
            continue;
        }

        // Simulate corruption
        if (rand() % CORRUPT_PROB == 0) {
            printf("Frame %d corrupted!\n\n", frame.seq);
            continue;
        }

        if (frame.seq == expectedSeq) {
            printf("Received frame %d: %s\n", frame.seq, frame.msg);
            int ack = frame.seq;
            send(newsockfd, &ack, sizeof(ack), 0);
            printf("Sent ACK %d\n\n", ack);
            expectedSeq = 1 - expectedSeq;
            count++;
        } else {
            // Duplicate frame, resend last ACK
            int lastAck = 1 - expectedSeq;
            send(newsockfd, &lastAck, sizeof(lastAck), 0);
            printf("Duplicate frame. Resent ACK %d\n\n", lastAck);
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
