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
        if (p.floor == floor) {
            img.draw_circle(p.x + ofs_x, p.y + ofs_y, 2, black);
            img.draw_text(p.x + ofs_x, p.y + ofs_y, p.id.c_str(), black);
            for (auto edge : p.edges) {
                if (points[edge.adj_point].floor != floor)
                    continue;
                img.draw_line(p.x + ofs_x, p.y + ofs_y,
                    points[edge.adj_point].x + ofs_x,
                    points[edge.adj_point].y + ofs_y, black);
            }
        }
    }

    img.save(output_file);
}

void Plan::draw_floor_plan (int floor, const char* output_file)
{
    int W = max_x[floor] + 5 * DELTA,  H = max_y[floor] + 5 * DELTA;
    int ofs_x = 2 * DELTA, ofs_y = 2 * DELTA;

    CImg<unsigned char> img(W, H, 1, 3);
    img.fill(255);
    unsigned char black[] = {0, 0, 0};

    vector<MotionDir> points_rotate(points.size());  /* где находится пункт
        по отношению к его координатам посередине коридора */
    for (auto path : paths) {
        for (auto point_index : path.path_points) {
            if (points[point_index].location == "left")
                points_rotate[point_index] = rotate_dir(path.dir, "left");
            else if (points[point_index].location == "right")
                points_rotate[point_index] = rotate_dir(path.dir, "right");
        }
    }

    for (int i = 0; i < points.size(); ++i) {
        Point& p = points[i];
        if (p.floor != floor)
            continue;
        int x = p.x + ofs_x, y = p.y + ofs_y;
        img.draw_circle(x, y, 2, black);
        if (!p.hidden) {
            int x0 = x, y0 = y;
            if (p.location == "left" || p.location == "right") {
                x0 = x + dx[points_rotate[i]] * DELTA;
                y0 = y + dy[points_rotate[i]] * DELTA;
            }
            img.draw_text(x0, y0, p.name.c_str(), black);
        }

        for (auto edge : p.edges) {
            if (points[edge.adj_point].floor != floor)
                continue;
            if (edge.reversed_path)
                continue;
            int dir = paths[edge.path].dir;
            /*if (paths[edge.path].path_points[0] == edge.adj_point
                || paths[edge.path].path_points[0] == i)
            {
                continue;
            }*/
            string path_type = paths[edge.path].path_type;
            switch (dir) {
            case UP:
                if (path_type == "left_right_walls" || path_type == "left_wall")
                    img.draw_line(x - DELTA, y - 2 * DELTA, x - DELTA, y, black);
                if (path_type == "left_right_walls" || path_type == "right_wall")
                    img.draw_line(x + DELTA, y - 2 * DELTA, x + DELTA, y, black);
                break;
            case LEFT:
                if (path_type == "left_right_walls" || path_type == "left_wall")
                    img.draw_line(x, y + DELTA, x - 2 * DELTA, y + DELTA, black);
                if (path_type == "left_right_walls" || path_type == "right_wall")
                    img.draw_line(x, y - DELTA, x - 2 * DELTA, y - DELTA, black);
                break;
            case RIGHT:
                if (path_type == "left_right_walls" || path_type == "left_wall")
                    img.draw_line(x, y - DELTA, x + 2 * DELTA, y - DELTA, black);
                if (path_type == "left_right_walls" || path_type == "right_wall")
                    img.draw_line(x, y + DELTA, x + 2 * DELTA, y + DELTA, black);
                break;
            case DOWN:
                if (path_type == "left_right_walls" || path_type == "left_wall")
                    img.draw_line(x - DELTA, y, x - DELTA, y + 2 * DELTA, black);
                if (path_type == "left_right_walls" || path_type == "right_wall")
                    img.draw_line(x + DELTA, y, x + DELTA, y + 2 * DELTA, black);
                break;
            }
        }
    }

    img.save(output_file);
}
