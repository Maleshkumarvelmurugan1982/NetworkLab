// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_NODES 10

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    int port;

    printf("Enter server port: ");
    scanf("%d", &port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    int n, source;
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    scanf("%d", &n);

    if (n > MAX_NODES || n <= 0) {
        printf("Invalid number of nodes.\n");
        return 1;
    }

    int graph[MAX_NODES][MAX_NODES];
    printf("Enter adjacency matrix (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &graph[i][j]);

    printf("Enter source node (0 to %d): ", n - 1);
    scanf("%d", &source);

    send(sock, &n, sizeof(n), 0);
    send(sock, &source, sizeof(source), 0);
    send(sock, graph, sizeof(int) * n * n, 0);

    int dist[MAX_NODES];
    recv(sock, dist, sizeof(int) * n, 0);

    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < n; i++) {
        if (dist[i] == 9999)
            printf("To %d: Unreachable\n", i);
        else
            printf("To %d: %d\n", i, dist[i]);
    }

    close(sock);
    return 0;
}
