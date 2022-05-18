#include <iostream>
#include "plan.h"

using namespace std;

int main()
{
    try {
        Plan plan;

        /*for (int i = 0; i < plan.points.size(); ++i) {
            cout << "point " << plan.points[i].id << endl;
            cout << "edges: ";
            for (int j = 0; j < plan.points[i].edges.size(); ++j)
                cout << plan.points[plan.points[i].edges[j].adj_point].id << " ; ";
            cout << endl;
        }*/


        plan.draw_floor_points(7, "7.bmp");
        plan.draw_floor_plan(7, "7_.bmp");
        plan.draw_floor_points(9, "9.bmp");
    }
    catch (const char* msg) {
        cerr << msg << endl;
        return -1;
    }

    return 0;
}
