#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <vector>
#include <list>

// ориентированный, не взвешенный граф
/*
    данные:
        n - число вершин в графе
        для i = 0..n-1:
            adj[i] - список смежности вершины i, т.е.
                список вершин { j | есть ребро i -> j }
    операции:
        конструктор (n - число вершин)
        добавление ребра в граф
        поиск кратчайшего пути из пункта a в пункт b (поиск в ширину)
*/
struct Graph {
    int n;  // число вершин
    std::vector<std::list<int> > adj;  // списки смежности вершин графа
    // для i = 0..n-1 : adj[i] - это list<int> - список вершин,
    //   в которые идет ребро из вершины i

    // путь в графе
    typedef std::vector<int> GraphPath;

    // конструктор класса Graph
    // создает граф с n вершинами
    Graph (int n_arg)
        : n(n_arg), adj(n_arg)  // adj - вектор из n пустых списков
    {}

    // добавить в граф ребро из вершины u в вершину v
    // (ребро из вершины v в вершину u не добавляется)
    void add_edge (int u, int v)
    {
        adj[u].push_back(v);
    }

    // найти кратчайший путь в графе из вершины a в вершину b
    //   path[0..k]:  path[0] = a, path[k] = b
    GraphPath find_shortest_path (int a, int b);
};

#endif // GRAPH_H_INCLUDED
