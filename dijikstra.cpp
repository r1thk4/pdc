#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>

using namespace std;

int INF = INT_MAX;

void serialDijkstra(vector<vector<int>> graph, int V, int src) {
    vector<int> dist(V, INF);
    vector<bool> visited(V, false);
    dist[src] = 0;
    for(int i=0; i<V-1; i++) {
        int u=-1;
        int minDist = INF;
        for(int v=0; v<V; v++) {
            if(!visited[v] && dist[v]<minDist) {
                minDist = dist[v];
                u = v;
            }
        }
        visited[u]=true;
        for(int v=0; v<V; v++) {
            if(!visited[v] && graph[u][v]!=0 && dist[u]!=INF && dist[v]>dist[u]+graph[u][v]) {
                dist[v] = graph[u][v] + dist[u];
            }
        }
    }
    cout<<"Serial: "<<endl;
    for(int i=0; i<V; i++) {
        cout<<dist[i]<<" ";
    }
}

void parallelDijikstra(vector<vector<int>> graph, int V, int src) {
    vector<int> dist(V, INF);
    vector<bool> visited(V, false);
    dist[src] = 0;
    for(int i=0; i<V-1; i++) {
        int u=-1;
        int minDist = INF;
        #pragma omp parallel
        {
            int local_u=-1;
            int local_min=INF;
            #pragma omp for
            for(int v=0; v<V; v++) {
                if(!visited[v] && dist[v]<local_min) {
                    local_min = dist[v];
                    local_u = v;
                }
                #pragma omp critical
                {
                    if(local_min<minDist) {
                        u = local_u;
                        minDist = local_min;
                    }
                }
            }
        }    
            visited[u]=true;
            #pragma omp parallel for
            for(int v=0; v<V; v++) {
                if(!visited[v] && graph[u][v]!=0 && dist[u]!=INF && dist[v]>dist[u]+graph[u][v]) {
                    #pragma omp critical 
                    {
                        dist[v] = graph[u][v] + dist[u];
                    }
                }
            }  
    }
    cout<<"Parallel: "<<endl;
    for(int i=0; i<V; i++) {
        cout<<dist[i]<<" ";
    }

}

int main() {
    vector<vector<int>> graph = {
        {0, 10, 0, 30, 100},
        {10, 0, 50, 0, 0},
        {0, 50, 0, 20, 10},
        {30, 0, 20, 0, 60},
        {100, 0, 10, 60, 0}
    };

    int V = graph.size();
    int src = 0;

    double start = omp_get_wtime();
    serialDijkstra(graph, V, src);
    double end = omp_get_wtime();

    cout << "\nTime taken: " << (end - start) << " seconds\n";

    start = omp_get_wtime();
    parallelDijikstra(graph, V, src);
    end = omp_get_wtime();

    cout << "\nTime taken: " << (end - start) << " seconds\n";

    return 0;

}