#include <stdio.h>
#include <omp.h>

#define INF 9999
#define V 5

// ---------- SERIAL VERSION ----------
void floydSerial(int graph[V][V]) {
    int dist[V][V];
    double start = omp_get_wtime();

    // Initialize distance matrix
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    // Floyd–Warshall algorithm (Serial)
    for (int k = 0; k < V; k++)
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];

    double end = omp_get_wtime();

    printf("\n=== Serial Floyd–Warshall Result ===\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                printf("%5s", "INF");
            else
                printf("%5d", dist[i][j]);
        }
        printf("\n");
    }

    printf("Serial Execution Time: %f seconds\n", end - start);
}

// ---------- PARALLEL VERSION USING OPENMP ----------
void floydParallel(int graph[V][V]) {
    int dist[V][V];
    double start = omp_get_wtime();

    // Initialize distance matrix
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    // Parallelized Floyd–Warshall
    for (int k = 0; k < V; k++) {
        #pragma omp parallel for shared(dist, k) collapse(2)
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    double end = omp_get_wtime();

    printf("\n=== Parallel Floyd–Warshall Result ===\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                printf("%5s", "INF");
            else
                printf("%5d", dist[i][j]);
        }
        printf("\n");
    }

    printf("Parallel Execution Time: %f seconds\n", end - start);
}

// ---------- MAIN FUNCTION ----------
int main() {
    int graph[V][V] = {
        {0, 3, 8, INF, -4},
        {INF, 0, INF, 1, 7},
        {INF, 4, 0, INF, INF},
        {2, INF, -5, 0, INF},
        {INF, INF, INF, 6, 0}
    };

    printf("Graph with negative weights (no negative cycles):\n");
    printf("1→2(3), 1→3(8), 1→5(-4), 2→4(1), 2→5(7), 3→2(4), 4→1(2), 4→3(-5), 5→4(6)\n\n");

    floydSerial(graph);
    floydParallel(graph);

    return 0;
}
