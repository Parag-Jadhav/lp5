#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h> // OpenMP header for parallelism

using namespace std;

// Graph class definition
class Graph {
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list representation of the graph

public:
    // Constructor to initialize graph with V vertices
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Adds an undirected edge between nodes u and v
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Because the graph is undirected
    }

    // Performs parallel Breadth-First Search (BFS) starting from the 'start' node
    void parallelBFS(int start) {
        vector<bool> visited(V, false); // Track visited nodes
        queue<int> q; // Standard queue for BFS

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS: ";

        while (!q.empty()) {
            int level_size = q.size(); // Number of nodes at current BFS level

            // Parallel loop to process all nodes at the current level
            #pragma omp parallel for
            for (int i = 0; i < level_size; i++) {
                int node;

                // Use critical section to safely access shared queue
                #pragma omp critical
                {
                    node = q.front();
                    q.pop();
                }

                cout << node << " ";

                // Visit all unvisited neighbors of the current node
                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        // Use critical section to avoid race condition
                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }

        cout << endl;
    }

    // Utility function for parallel DFS (called recursively)
    void parallelDFSUtil(int node, vector<bool>& visited) {
        visited[node] = true;
        cout << node << " ";

        // Explore all adjacent nodes in parallel
        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {
            int neighbor = adj[node][i];
            if (!visited[neighbor]) {
                // Spawn a new task for each unvisited neighbor
                #pragma omp task
                parallelDFSUtil(neighbor, visited);
            }
        }

        // Wait for all spawned tasks to finish
        #pragma omp taskwait
    }

    // Wrapper function for starting parallel DFS from a given node
    void parallelDFS(int start) {
        vector<bool> visited(V, false); // Track visited nodes
        cout << "Parallel DFS: ";

        // Start the OpenMP parallel region with a single task
        #pragma omp parallel
        {
            #pragma omp single
            parallelDFSUtil(start, visited);
        }

        cout << endl;
    }
};

int main() {
    Graph g(7); // Create a graph with 7 vertices

    // Add edges to form a binary tree-like structure
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    double t1, t2;

    // Run and display the result of parallel BFS
    g.parallelBFS(0);

    // Example for timing (commented out)
    // t1 = omp_get_wtime();
    // g.parallelBFS(0);
    // t2 = omp_get_wtime();
    // cout << "Sequential Merge Sort:  " << (t2 - t1) << " s\n";

    // Run and display the result of parallel DFS
    t1 = omp_get_wtime();
    g.parallelDFS(0);
    t2 = omp_get_wtime();

    // Example for timing (commented out)
    // cout << "Sequential Merge Sort:  " << (t2 - t1) << " s\n";

    return 0;
}
