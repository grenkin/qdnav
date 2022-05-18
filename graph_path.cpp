/*
    Модуль поиска путей в графе

    Реализации методов
        Graph
            find_shortest_path
*/

#include <stack>
#include <queue>
#include <algorithm>
#include "graph.h"

using namespace std;

// найти кратчайший путь в графе из вершины a в вершину b
//   path[0..k]:  path[0] = a, path[k] = b
Graph::GraphPath Graph::find_shortest_path (int a, int b)
{
    queue<int> q;
    vector<bool> visited(n);
    vector<int> distance(n), source(n);
    q.push(a);
    visited[a] = true;
    source[a] = -1;

    while (!q.empty()) {
        int from = q.front();
        q.pop();

        for (auto to: adj[from]) {
            if (!visited[to]) {
                distance[to] = distance[from] + 1;
                visited[to] = true;
                source[to] = from;
                q.push(to);
            }
        }
    }

    GraphPath path;
    if (visited[b]) {
        for (int v = b; v != -1; v = source[v])
            path.push_back(v);
        reverse(path.begin(), path.end());
    }

    return path;
}
