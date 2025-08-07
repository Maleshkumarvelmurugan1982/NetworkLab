// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAX_NODES 10
#define INF 9999

void dijkstra(int **graph, int n, int source, int *dist) {
    int visited[MAX_NODES] = {0};

    for (int i = 0; i < n; i++)
        dist[i] = INF;

    dist[source] = 0;

    for (int count = 0; count < n - 1; count++) {
        int min = INF, u = -1;

        for (int v = 0; v < n; v++)
            if (!visited[v] && dist[v] <= min)
                min = dist[v], u = v;

        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < n; v++)
            if (!visited[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int port;

    printf("Enter port to listen on: ");
    scanf("%d", &port);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 3);
    printf("Server listening on port %d...\n", port);

    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    printf("Client connected!\n");

    int n, source;
    recv(new_socket, &n, sizeof(n), 0);

    int **graph = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        graph[i] = (int*)malloc(n * sizeof(int));

    recv(new_socket, &source, sizeof(source), 0);
    recv(new_socket, *graph, sizeof(int) * n * n, 0);

    int *dist = (int*)malloc(n * sizeof(int));
    dijkstra(graph, n, source, dist);

    send(new_socket, dist, sizeof(int) * n, 0);
    printf("Distances sent.\n");

    for (int i = 0; i < n; i++) free(graph[i]);
    free(graph);
    free(dist);

    close(new_socket);
    close(server_fd);
    return 0;
}
