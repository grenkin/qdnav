#include <algorithm>
#include "plan.h"

using namespace std;

// обходим все пути, при этом заполняем массив edges и считаем координаты
void Plan::process_plan ()
{
    vector<bool> points_visited(points.size());
    // инициализация: заполняем координаты нулями
    for (auto& p : points) {
        p.x = 0;  p.y = 0;
    }

    /*
    // устанавливаем координаты у самой первой точки
    int first_point = paths[0].path_points[0];
    points[first_point]->x = 1;  points[first_point]->y = 1;
    points_visited[first_point] = true;
    */

    for (int path_index = 0; path_index < paths.size(); ++path_index) {
        Path& path = paths[path_index];
        for (int i = 1; i < path.path_points.size(); ++i) {
            int point_index = path.path_points[i];
            int prev_point_index = path.path_points[i - 1];
            MotionDir dir = path.dir;

            if (!points_visited[point_index]) {
              //  if (points[point_index].point_type == PT_WALL /*||
               //     points[point_index].point_type == PT_DOOR*/)
              //  {
              //      points[point_index].x = points[prev_point_index].x;
              //      points[point_index].y = points[prev_point_index].y;
              //  }
             //   else {
                    points[point_index].x = points[prev_point_index].x
                        + 2 * DELTA * dx[dir];
                    points[point_index].y = points[prev_point_index].y
                        + 2 * DELTA * dy[dir];
            //    }
                points_visited[point_index] = true;
            }

            // добавляем ребра между точками (i-1), i и между i, (i-1)
            Edge edge1;
            edge1.adj_point = point_index;
            edge1.path = path_index;
            edge1.reversed_path = false;
            points[prev_point_index].edges.push_back(edge1);
            Edge edge2;
            edge2.adj_point = prev_point_index;
            edge2.path = path_index;
            edge2.reversed_path = true;
            points[point_index].edges.push_back(edge2);
        }
    }

    // перебираем все лестницы и лифты, проводим ребра
    for (auto [id, points_vec] : stairs_by_id) {
        for (int i = 0; i < points_vec.size(); ++i) {
            for (int j = i + 1; j < points_vec.size(); ++j) {
                // добавляем ребра между точками i, j и между j, i
                Edge edge1;
                edge1.adj_point = points_vec[j];
                edge1.path = -1;
                edge1.reversed_path = false;
                points[points_vec[i]].edges.push_back(edge1);
                Edge edge2;
                edge2.adj_point = points_vec[i];
                edge2.path = -1;
                edge2.reversed_path = false;
                points[points_vec[j]].edges.push_back(edge2);
            }
        }
    }
    for (auto [id, points_vec] : elevators_by_id) {
        for (int i = 0; i < points_vec.size(); ++i) {
            for (int j = i + 1; j < points_vec.size(); ++j) {
                // добавляем ребра между точками i, j и между j, i
                Edge edge1;
                edge1.adj_point = points_vec[j];
                edge1.path = -2;
                edge1.reversed_path = false;
                points[points_vec[i]].edges.push_back(edge1);
                Edge edge2;
                edge2.adj_point = points_vec[i];
                edge2.path = -2;
                edge2.reversed_path = false;
                points[points_vec[j]].edges.push_back(edge2);
            }
        }
    }

    std::map<int, int> min_x, min_y;  // минимальные координаты для каждого этажа
    // приведение координат к положительным
    for (auto p : points) {
        max_x[p.floor] = max(max_x[p.floor], p.x);
        max_y[p.floor] = max(max_y[p.floor], p.y);
        min_x[p.floor] = min(min_x[p.floor], p.x);
        min_y[p.floor] = min(min_y[p.floor], p.y);
    }
    for (auto& p : points) {
        p.x -= min_x[p.floor];
        p.y -= min_y[p.floor];
    }
    for (auto p : points) {
        max_x[p.floor] = max_x[p.floor] - min_x[p.floor];
        min_x[p.floor] = 0;
        max_y[p.floor] = max_y[p.floor] - min_y[p.floor];
        min_y[p.floor] = 0;
    }
}
