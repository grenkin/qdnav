#ifndef PLAN_H_INCLUDED
#define PLAN_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "pugixml.hpp"

struct Edge {  // ребро -- это единичный отрезок пути
    int adj_point;  // номера пунктов, в которые можно пройти
    int path;  // для каждого пункта хранится номер соответствующего пути
        // либо -1, если движение по лестнице или -2, если по лифту
    bool reversed_path;  // true, если путь нужно пройти в обратном порядке
};

enum PointType {
    PT_POINT, PT_EXIT, PT_ROOM, PT_TOILET, PT_STAIRS, PT_ELEVATOR, PT_WALL,
    PT_DOOR
};

struct Point {
    PointType point_type;
    std::string id;
    std::string name;
    std::string location;
    bool hidden;
    int floor;
    std::vector<Edge> edges;  // список смежных ребер
    int x, y;  // координаты пункта при отрисовке

    Edge get_edge (int adj_point_index);
};

enum MotionDir {
    UP, RIGHT, DOWN, LEFT
};

struct Path {
    MotionDir dir;  // направление пути на карте
    std::string path_type;  // no_walls, left_wall, right_wall, left_right_walls
    std::vector<int> path_points;  // номера пунктов в массиве points
};

class Plan {
    // массивы соответствуют enum MotionDir
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {-1, 0, 1, 0};

    const int DELTA = 50;  // половина единичного отрезка в пикселях при отрисовке
    std::map<std::string, int> points_by_id;  /* для каждого id хранится
        индекс точки в массиве points */
    std::map<std::string, std::vector<int>> stairs_by_id;  /* для каждой лестницы
        список номеров пунктов */
    std::map<std::string, std::vector<int>> elevators_by_id;  /* для каждого лифта
        список номеров пунктов */
    std::map<int, int> max_x, max_y;  // максимальные координаты для каждого этажа

    int add_point (Point& point);
    MotionDir rotate_dir (MotionDir dir, std::string rotate_str);
    std::string get_stairs_point_id (std::string stairs_id, int floor);
    std::string get_elevator_point_id (std::string elevator_id, int floor);
    int get_point_by_id (std::string point_id, int floor);
    void process_xml_node (pugi::xml_node& node, int path_index, int floor_num);
    void read_path (pugi::xml_node& node, int path_index, int floor_num);
    void read_plan ();
    void process_plan ();

public:
    std::vector<Point> points;
    std::vector<Path> paths;

    Plan ()
    {
        read_plan();
        process_plan();
    }
    std::vector<int> find_route (std::string from_id, std::string to_id);
    void process_route (std::vector<int> route);
    void draw_floor_points (int floor, const char* output_file);
    void draw_floor_plan (int floor, const char* output_file);
};

#endif // PLAN_H_INCLUDED
