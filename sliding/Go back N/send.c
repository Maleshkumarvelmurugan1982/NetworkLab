#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define PORT 2030
#define MAXSZ 100
#define MAX_FRAMES 30

typedef struct {
    int seq;
    char data[MAXSZ];
} Frame;

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    Frame frames[MAX_FRAMES];
    char buffer[MAXSZ];
    int SF = 0, S = 0, SL;
    int numFrames, windowSize;

    // Prepare frames
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames[i].seq = i % 8; // 3-bit sequence
        sprintf(frames[i].data, "Frame_data_%d", i);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket failed"); exit(1); }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connect failed"); exit(1);
    }

    printf("Enter number of frames to send (max %d): ", MAX_FRAMES);
    scanf("%d", &numFrames);

    printf("Enter window size: ");
    scanf("%d", &windowSize);
    if (windowSize < 1 || windowSize > 7) {
        printf("Window size must be between 1 and 7 for 3-bit sequence\n");
        return 0;
    }

    while (SF < numFrames) {
        SL = SF + windowSize - 1;
        if (SL >= numFrames) SL = numFrames - 1;

        // Send all frames in window
        for (S = SF; S <= SL; S++) {
            sprintf(buffer, "%d%s", frames[S].seq, frames[S].data);
            send(sockfd, buffer, strlen(buffer) + 1, 0);
            printf("Sent frame %d\n", frames[S].seq);
        }

        // Wait for ACK
        char ack;
        int n = recv(sockfd, &ack, 1, 0);
        if (n > 0) {
            int ackNum = ack - '0';
            printf("Received ACK %d\n", ackNum);

            if ((ackNum - SF + 8) % 8 > 0) {
                SF = ackNum; // slide window
            } else {
                printf("Timeout or duplicate ACK, resending frames from %d\n", SF);
            }
        } else {
            printf("No ACK received, resending frames from %d\n", SF);
        }
    }

    printf("All frames sent successfully.\n");
    close(sockfd);
    return 0;
}
