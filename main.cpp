#include <iostream>
#include <fstream>
#include "plan.h"

using namespace std;

int main()
{
    try {
        Plan plan;
        ifstream fin("input.txt");
        string point_from, point_to;
        fin >> point_from >> point_to;
        cout << "From " << point_from << " to " << point_to << endl;
        vector<int> route = plan.find_route(point_from, point_to);
        plan.process_route(route);

        /*for (int i = 0; i < plan.points.size(); ++i) {
            cout << "point " << plan.points[i].id << endl;
            cout << "edges: ";
            for (int j = 0; j < plan.points[i].edges.size(); ++j)
                cout << plan.points[plan.points[i].edges[j].adj_point].id << " ; ";
            cout << endl;
        }*/
        /*
        for (int i = 0; i < plan.paths.size(); ++i) {
            cout << "path " << i << endl;
            for (int j = 0; j < plan.paths[i].path_points.size(); ++j) {
                //cout << plan.points[plan.paths[i].path_points[j]].id << ", ";
                cout << plan.paths[i].path_points[j] << ", ";
            }
            cout << endl;
        }
        */

        plan.draw_floor_plan(3, "3.bmp");

    }
    catch (const char* msg) {
        cerr << msg << endl;
        return -1;
    }

    return 0;
}
