#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define V 10  // max number of vertices

int main() {
    int n, src;
    int graph[V][V];

    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix (use 999 if no edge):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
            if (graph[i][j] == 999) {
                graph[i][j] = 0; // treat 999 as no edge
            }
        }
    }

    printf("Enter source vertex: ");
    scanf("%d", &src);

    while (1) {
        int dist[V], parent[V];
        bool visited[V];

        // initialize arrays
        for (int i = 0; i < n; i++) {
            dist[i] = INT_MAX;
            visited[i] = false;
            parent[i] = -1;
        }
        dist[src] = 0;

        // dijkstra core
        for (int count = 0; count < n - 1; count++) {
            int min = INT_MAX, u = -1;

            for (int v = 0; v < n; v++) {
                if (!visited[v] && dist[v] <= min) {
                    min = dist[v];
                    u = v;
                }
            }

            if (u == -1) break; // no more reachable nodes
            visited[u] = true;

            for (int v = 0; v < n; v++) {
                if (!visited[v] && graph[u][v] && dist[u] != INT_MAX &&
                    dist[u] + graph[u][v] < dist[v]) {
                    parent[v] = u;
                    dist[v] = dist[u] + graph[u][v];
                }
            }
        }

        // print results
        printf("\n--- Shortest paths from %d ---\n", src);
        printf("Vertex\tDistance\tPath");
        for (int i = 0; i < n; i++) {
            if (i == src) continue;
            printf("\n%d -> %d\t", src, i);
            if (dist[i] == INT_MAX) {
                printf("INF\t\tNo path");
            } else {
                printf("%d\t\t", dist[i]);
                // print path
                int path[V], len = 0;
                int j = i;
                while (j != -1) {
                    path[len++] = j;
                    j = parent[j];
                }
                for (int k = len - 1; k >= 0; k--) {
                    printf("%d", path[k]);
                    if (k > 0) printf(" -> ");
                }
            }
        }
        printf("\n");

        int choice;
        printf("\nDo you want to remove a connection? (1-Yes, 0-No): ");
        scanf("%d", &choice);
        if (choice == 0) break;

        int u, v;
        printf("Enter edge to remove (u v): ");
        scanf("%d %d", &u, &v);

        if (u >= 0 && u < n && v >= 0 && v < n) {
            graph[u][v] = 0; // remove edge
            graph[v][u] = 0; // if undirected
            printf("Edge (%d, %d) removed.\n", u, v);
        } else {
            printf("Invalid edge!\n");
        }
    }

    return 0;
}
