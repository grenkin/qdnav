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
    int floor;
    std::vector<Edge> edges;  // список смежных ребер
    int x, y;  // координаты пункта при отрисовке

    Point() { point_type = PT_POINT; }
};

struct Exit : public Point {
     Exit() { point_type = PT_EXIT; }
};

struct Wall : public Point {
     Wall() { point_type = PT_WALL; }
     Wall(Point& point) : Point(point) { point_type = PT_WALL; }
};

struct Door : public Point {
     Door() { point_type = PT_DOOR; }
     Door(Point& point) : Point(point) { point_type = PT_DOOR; }
};

struct Room : public Point {
    //std::string location;  // left, right

    Room() { point_type = PT_ROOM; name = id; }
    Room(Point& point) : Point(point) { point_type = PT_ROOM; name = id; }
};

struct Toilet : public Room {
    Toilet() { point_type = PT_TOILET; }
    Toilet(Point& point) : Room(point) { point_type = PT_TOILET; }
};

struct Stairs : public Point {
    //std::string location;  // left, right, along

    Stairs() { point_type = PT_STAIRS; }
    Stairs(Point& point) : Point(point) { point_type = PT_STAIRS; }
};

struct Elevator : public Point {
    //std::string location;  // left, right, along

    Elevator() { point_type = PT_ELEVATOR; }
    Elevator(Point& point) : Point(point) { point_type = PT_ELEVATOR; }
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
    const int DELTA = 50;  // половина единичного отрезка в пикселях при отрисовке
    std::map<std::string, int> points_by_id;  /* для каждого id хранится
        индекс точки в массиве points */
    std::map<std::string, std::vector<int>> stairs_by_id;  /* для каждой лестницы
        список номеров пунктов */
    std::map<std::string, std::vector<int>> elevators_by_id;  /* для каждого лифта
        список номеров пунктов */
    std::map<int, int> max_x, max_y;  // максимальные координаты для каждого этажа

    int add_point (std::shared_ptr<Point> point);
    std::string get_stairs_point_id (std::string stairs_id, int floor);
    std::string get_elevator_point_id (std::string elevator_id, int floor);
    int get_point_by_id (std::string point_id, int floor);
    void process_xml_node (pugi::xml_node& node, int path_index, int floor_num);
    void read_path (pugi::xml_node& node, int path_index, int floor_num);
    void read_plan ();
    void process_plan ();

public:
    std::vector<std::shared_ptr<Point>> points;
    std::vector<Path> paths;

    Plan ()
    {
        read_plan();
        process_plan();
    }
    void draw_floor_points (int floor, const char* output_file);
    void draw_floor_plan (int floor, const char* output_file);
};

#endif // PLAN_H_INCLUDED
