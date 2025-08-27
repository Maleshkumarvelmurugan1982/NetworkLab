#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT  9876
#define MAXSZ 1024

typedef struct {
    int seq;         // sequence number 0 or 1
    int len;
    char msg[MAXSZ];
} Frame;

Frame* makeFrames(int total) {
    Frame* fr = (Frame*) malloc(total * sizeof(Frame));
    for (int i = 0; i < total; i++) {
        fr[i].seq = i % 2; // alternate 0,1
        snprintf(fr[i].msg, MAXSZ, "Message %d", i+1);
        fr[i].len = strlen(fr[i].msg);
    }
    return fr;
}

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    Frame* frames = makeFrames(30);
    int totalFrames;

    printf("Enter number of frames to send (max 30): ");
    scanf("%d", &totalFrames);
    if (totalFrames > 30) totalFrames = 30;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Socket"); exit(1); }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connect"); exit(1);
    }

    int count = 0;
    while (count < totalFrames) {
        Frame f = frames[count];
        printf("Sending frame %d: %s\n", f.seq, f.msg);
        send(sockfd, &f, sizeof(Frame), 0);

        int ack;
        // wait for ACK
        int n = recv(sockfd, &ack, sizeof(ack), 0);
        if (n <= 0) {
            printf("No ACK received. Resending frame %d\n\n", f.seq);
            continue;
        }

        if (ack == f.seq) {
            printf("ACK %d received. Moving to next frame.\n\n", ack);
            count++;
        } else {
            printf("Wrong ACK %d received. Resending frame %d\n\n", ack, f.seq);
        }
    }

    printf("All frames sent successfully!\n");
    close(sockfd);
    free(frames);
    return 0;
}
