#include <iostream>
#include "plan.h"

using namespace std;

int main()
{
    try {
        Plan plan;
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
