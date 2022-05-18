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

Edge Point::get_edge (int adj_point_index)
{
    for (int i = 0; i < edges.size(); ++i) {
        if (edges[i].adj_point == adj_point_index)
            return edges[i];
    }
}

void Plan::process_route (vector<int> route)
{
    ofstream fout("output.txt");
    /*for (int i = 0; i < route.size(); ++i) {
        fout << points[route[i]].id << "  " << points[route[i]].name << endl;
    }*/
    int i = 0, j;
    Edge edge = points[route[0]].get_edge(route[1]);
    int path_index = edge.path;
    bool reversed_path = edge.reversed_path;
    while (i < route.size() && j < route.size()) {
        // нас интересует отрезок маршрута от i-й вершины до j-й,
        // на котором ребра маршрута из одного и того же пути
        j = i + 1;
        int new_path_index;
        bool reversed_new_path;

        while (j < route.size()) {
            Edge edge = points[route[j - 1]].get_edge(route[j]);
            new_path_index = edge.path;
            reversed_new_path = edge.reversed_path;
            if (new_path_index != path_index) {
                j--;
                break;
            }
            ++j;
        }
        if (j == route.size())
            j--;

        if (path_index == -1) {
            fout << "Пройти по лестнице." << endl;
        }
        else if (path_index == -2) {
            fout << "Проехать на лифте." << endl;
        }
        else {
            fout << "Пройти от " << points[route[i]].name << " до "
                << points[route[j]].name;
            if (j > i + 1) {
                bool hidden = true;
                for (int k = i + 1; k <= j - 1; ++k) {
                    if (!points[route[k]].hidden) {
                        hidden = false;
                        break;
                    }
                }
                if (!hidden) {
                    fout << " мимо ";
                    bool flag = false;
                    for (int k = i + 1; k <= j - 1; ++k) {
                        if (!points[route[k]].hidden) {
                            if (flag)
                                fout << ", ";
                            flag = true;
                            fout << points[route[k]].name;
                        }
                    }
                }
            }
            fout << "." << endl;
            if (new_path_index >= 0) {
                MotionDir dir1 = paths[new_path_index].dir;
                if (reversed_new_path)
                    dir1 = rotate_dir(dir1, "back");
                MotionDir dir2 = paths[path_index].dir;
                if (reversed_path)
                    dir2 = rotate_dir(dir2, "back");
                MotionDir d = (MotionDir)((dir1 - dir2 + 4) % 4);
                if (d > UP) {
                    switch (d) {
                    case LEFT:
                        fout << "Поверните налево." << endl;
                        break;
                    case DOWN:
                        fout << "Поверните назад." << endl;
                        break;
                    case RIGHT:
                        fout << "Поверните направо." << endl;
                        break;
                    }
                }
            }
        }

        path_index = new_path_index;
        reversed_path = reversed_new_path;
        i = j;
        if (i == route.size() - 1)
            break;
    }

}
