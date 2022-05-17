#include "plan.h"
#define cimg_display 0
#include "CImg.h"

using namespace std;
using namespace cimg_library;

void Plan::draw_floor_points (int floor, const char* output_file)
{
    int W = max_x[floor] + 2 * DELTA,  H = max_y[floor] + 2 * DELTA;
    int ofs_x = DELTA, ofs_y = DELTA;

    CImg<unsigned char> img(W, H, 1, 3);
    img.fill(255);
    unsigned char black[] = {0, 0, 0};

    for (auto p : points) {
        if (p->floor == floor) {
            img.draw_circle(p->x + ofs_x, p->y + ofs_y, 2, black);
            img.draw_text(p->x + ofs_x, p->y + ofs_y, p->id.c_str(), black);
            for (auto edge : p->edges) {
                if (points[edge.adj_point]->floor != floor)
                    continue;
                img.draw_line(p->x + ofs_x, p->y + ofs_y,
                    points[edge.adj_point]->x + ofs_x,
                    points[edge.adj_point]->y + ofs_y, black);
            }
        }
    }

    img.save(output_file);
}

void Plan::draw_floor_plan (int floor, const char* output_file)
{
    int W = max_x[floor] + 2 * DELTA,  H = max_y[floor] + 2 * DELTA;
    int ofs_x = DELTA, ofs_y = DELTA;

    CImg<unsigned char> img(W, H, 1, 3);
    img.fill(255);
    unsigned char black[] = {0, 0, 0};

    for (auto p : points) {
        if (p->floor == floor) {
            int x = p->x + ofs_x, y = p->y + ofs_y;
            for (auto edge : p->edges) {
                if (points[edge.adj_point]->floor != floor)
                    continue;
                switch (paths[edge.path].dir) {
                case UP:
                    if (paths[edge.path].path_type == "left_right_walls" ||
                        paths[edge.path].path_type == "left_wall")
                    {
                        img.draw_line(p->x - DELTA + ofs_x,
                            p->y - DELTA + ofs_y,
                            p->x - DELTA + ofs_x,
                            p->y + DELTA + ofs_y, black);
                        if (p->location == "left") {
                            img.draw_text(p->x - DELTA + ofs_x, p->y + ofs_y,
                                p->name.c_str(), black);
                        }
                    }
                    if (paths[edge.path].path_type == "left_right_walls" ||
                        paths[edge.path].path_type == "right_wall")
                    {
                        img.draw_line(p->x + DELTA + ofs_x,
                            p->y - DELTA + ofs_y,
                            p->x + DELTA + ofs_x,
                            p->y + DELTA + ofs_y, black);
                        if (p->location == "right") {
                            img.draw_text(p->x + DELTA + ofs_x, p->y + ofs_y,
                                p->name.c_str(), black);
                        }
                    }
                    break;
                case LEFT:
                    if (paths[edge.path].path_type == "left_right_walls" ||
                        paths[edge.path].path_type == "left_wall")
                    {
                        img.draw_line(p->x - DELTA + ofs_x,
                            p->y + DELTA + ofs_y,
                            p->x + DELTA + ofs_x,
                            p->y + DELTA + ofs_y, black);
                        if (p->location == "left") {
                            img.draw_text(p->x + ofs_x, p->y + DELTA + ofs_y,
                                p->name.c_str(), black);
                        }
                    }
                    if (paths[edge.path].path_type == "left_right_walls" ||
                        paths[edge.path].path_type == "right_wall")
                    {
                        img.draw_line(p->x - DELTA + ofs_x,
                            p->y - DELTA + ofs_y,
                            p->x + DELTA + ofs_x,
                            p->y - DELTA + ofs_y, black);
                        if (p->location == "right") {
                            img.draw_text(p->x + ofs_x, p->y - DELTA + ofs_y,
                                p->name.c_str(), black);
                        }
                    }
                    break;
                case RIGHT:
                    break;
                case DOWN:
                    break;
                }
            }
           /* for (auto edge : p->edges) {
                if (points[edge.adj_point]->floor != floor)
                    continue;
                img.draw_line(p->x + ofs_x, p->y + ofs_y,
                    points[edge.adj_point]->x + ofs_x,
                    points[edge.adj_point]->y + ofs_y, black);
            } */
        }
    }

    img.save(output_file);
}
