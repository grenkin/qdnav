#include <fstream>
#include "plan.h"
#include "graph.h"

using namespace std;

vector<int> Plan::find_route (std::string from_id, std::string to_id)
{
    Graph graph(points.size());
    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points[i].edges.size(); ++j)
            graph.add_edge(i, points[i].edges[j].adj_point);
    }
    int from_index = points_by_id[from_id], to_index = points_by_id[to_id];
    Graph::GraphPath route = graph.find_shortest_path(from_index, to_index);
    return route;
}

void Plan::process_route (vector<int> route)
{
    ofstream fout("output.txt");
    for (int i = 0; i < route.size(); ++i) {
        fout << points[route[i]].id << endl;
    }
}
