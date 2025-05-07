#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Graph class representing the adjacency list
class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    void parallelDFS(int startVertex) {
        vector<bool> visited(V, false);
        parallelDFSUtil(startVertex, visited);
    }

    void parallelDFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        #pragma omp parallel for
        for (int i = 0; i < adj[v].size(); ++i) {
            int n = adj[v][i];
            if (!visited[n])
                parallelDFSUtil(n, visited);
        }
    }

    void parallelBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            #pragma omp parallel for
            for (int i = 0; i < adj[v].size(); ++i) {
                int n = adj[v][i];
                if (!visited[n]) {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }
    }
};

int main() {
    int numVertices, numEdges, startVertex;
    
    cout << "Enter the number of vertices: ";
    cin >> numVertices;
    
    Graph g(numVertices);
    
    cout << "Enter the number of edges: ";
    cin >> numEdges;
    
    cout << "Enter " << numEdges << " edges (from_vertex to_vertex):" << endl;
    for (int i = 0; i < numEdges; i++) {
        int from, to;
        cin >> from >> to;
        // Check if vertices are valid
        if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
            g.addEdge(from, to);
        } else {
            cout << "Invalid vertices. Vertices should be between 0 and " << (numVertices-1) << endl;
            i--; // Let the user enter this edge again
        }
    }
    
    cout << "Enter the starting vertex for BFS and DFS: ";
    cin >> startVertex;
    
    if (startVertex < 0 || startVertex >= numVertices) {
        cout << "Invalid starting vertex. Using vertex 0 instead." << endl;
        startVertex = 0;
    }

    cout << "Depth-First Search (DFS): ";
    g.parallelDFS(startVertex);
    cout << endl;

    cout << "Breadth-First Search (BFS): ";
    g.parallelBFS(startVertex);
    cout << endl;

    return 0;
}