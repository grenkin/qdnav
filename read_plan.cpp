#include "plan.h"

using namespace std;
using namespace pugi;

int Plan::add_point (shared_ptr<Point> point)
{
    points.push_back(point);
    int point_index = points.size() - 1;
    points_by_id[point->id] = point_index;
    return point_index;
}

MotionDir rotate_dir (MotionDir dir, string rotate_str)
{
    MotionDir new_dir;
    if (rotate_str == "forward")
        new_dir = dir;
    else if (rotate_str == "left")
        new_dir = (MotionDir)((int(dir) + 3) % 4);
    else if (rotate_str == "right")
        new_dir = (MotionDir)((int(dir) + 1) % 4);
    else if (rotate_str == "back")
        new_dir = (MotionDir)((int(dir) + 2) % 4);
    else
        throw "Bad rotate_str";
    return new_dir;
}

std::string Plan::get_stairs_point_id (std::string stairs_id, int floor)
{
    return stairs_id + "_stairs_" + to_string(floor);
}

std::string Plan::get_elevator_point_id (std::string elevator_id, int floor)
{
    return elevator_id + "_elevator_" + to_string(floor);
}

int Plan::get_point_by_id (string point_id, int floor)
{
    int point_index;
    // проверяем, есть ли лестница или лифт с таким id
    string stairs_point_id = get_stairs_point_id(point_id, floor);
    if (points_by_id.find(stairs_point_id) != points_by_id.end())
        // ключ stairs_point_id присутствует
    {
        point_index = points_by_id[stairs_point_id];
    }
    else
        point_index = points_by_id[point_id];
    return point_index;
}

// прочитать содержимое тега (не <Path>) и сохранить в путь с индексом path_index
// если path_index == -1, то не сохранять как путь
void Plan::process_xml_node (xml_node& node, int path_index, int floor_num)
    // node.name() != "Path"
{
    Point point0;
    point0.id = node.attribute("id").value();
    point0.name = node.attribute("name").value();
    point0.floor = floor_num;
    int point_index;
    // TODO: разобрать случай, когда точка с таким id уже есть

    string node_name = node.name();
    if (node_name == "Point") {
        shared_ptr<Point> point(new Point(point0));
        point_index = add_point(point);
    }
    else if (node_name == "Room") {
        shared_ptr<Room> room(new Room(point0));
        room->location = node.attribute("location").value();
        point_index = add_point(room);
    }
    else if (node_name == "Toilet") {
        shared_ptr<Toilet> toilet(new Toilet(point0));
        toilet->location = node.attribute("location").value();
        point_index = add_point(toilet);
    }
    else if (node_name == "Stairs") {
        shared_ptr<Stairs> stairs(new Stairs(point0));
        stairs->location = node.attribute("location").value();
        string stairs_id = stairs->id;
        string stairs_point_id =
            get_stairs_point_id(stairs_id, floor_num);
        stairs->id = stairs_point_id;
        point_index = add_point(stairs);
        stairs_by_id[stairs_id].push_back(point_index);
    }
    else if (node_name == "Elevator") {
        shared_ptr<Elevator> elevator(new Elevator(point0));
        elevator->location = node.attribute("location").value();
        string elevator_id = elevator->id;
        string elevator_point_id =
            get_elevator_point_id(elevator->id, floor_num);
        elevator->id = elevator_point_id;
        point_index = add_point(elevator);
        elevators_by_id[elevator_id].push_back(point_index);
    }
    else if (node_name == "Wall") {
        shared_ptr<Wall> wall(new Wall(point0));
        point_index = add_point(wall);
    }
    else if (node_name == "Door") {
        shared_ptr<Door> door(new Door(point0));
        point_index = add_point(door);
    }
    if (path_index != -1) {
        // добавить пункт в список пунктов данного пути
        paths[path_index].path_points.push_back(point_index);
    }
}

// прочитать содержимое тега <Path> и сохранить в путь с индексом path_index
void Plan::read_path (xml_node& node, int path_index, int floor_num)
    // node.name() == "Path"
{
    for (xml_node node_child = node.first_child(); node_child;
        node_child = node_child.next_sibling())
    {
        string node_name = node_child.name();
        if (node_name == "Path") {
            // вложенный тег Path означает, что вложенный путь определенным
            // образом ориентирован относительно родительского пути

            // добавляем новый путь
            paths.push_back(Path());
            Path& new_path = paths.back();
            int new_path_index = paths.size() - 1;

            // разбираем атрибуты пути в xml и заполняем нашу структуру данных
            string point_id = node_child.attribute("start").value();
            int point_index = get_point_by_id(point_id, floor_num);
            // добавляем пункт в список пунктов пути
            new_path.path_points.push_back(point_index);
            string path_rotate_str =
                node_child.attribute("rotate").value();
            MotionDir parent_path_dir = paths[path_index].dir;
            new_path.dir = rotate_dir(parent_path_dir, path_rotate_str);
            new_path.path_type = node_child.attribute("type").value();

            read_path(node_child, new_path_index, floor_num);
        }
        else {
            process_xml_node(node_child, path_index, floor_num);
        }
    }
}

void Plan::read_plan ()
{
    xml_document doc;
    if (!doc.load_file("Plan.xml"))
        throw "Can't open Plan.xml";
    xml_node plan = doc.child("Plan");
    for (xml_node plan_child = plan.first_child(); plan_child;
        plan_child = plan_child.next_sibling())
    {
        if (string(plan_child.name()) == "Exit") {
            int exit_floor = plan_child.attribute("floor").as_int();
            string exit_id = plan_child.attribute("id").value();
            string exit_name = plan_child.attribute("name").value();
            shared_ptr<Exit> exit(new Exit());
            exit->id = exit_id;
            exit->name = exit_name;
            exit->floor = exit_floor;
            add_point(exit);
        }
        else if (string(plan_child.name()) == "Floor") {
            int floor_num = plan_child.attribute("num").as_int();
            for (xml_node floor_child = plan_child.first_child(); floor_child;
                floor_child = floor_child.next_sibling())
            {
                if (string(floor_child.name()) == "Path") {
                    // добавляем новый путь
                    paths.push_back(Path());
                    Path& path = paths.back();
                    int path_index = paths.size() - 1;
                    // считываем атрибуты пути из xml-файла
                    path.path_type = floor_child.attribute("type").value();
                    string dir_str = floor_child.attribute("dir").value();
                    MotionDir dir;
                    if (dir_str == "up")
                        dir = UP;
                    else if (dir_str == "down")
                        dir = DOWN;
                    else if (dir_str == "left")
                        dir = LEFT;
                    else if (dir_str == "right")
                        dir = RIGHT;
                    path.dir = dir;

                    // добавляем точку в путь
                    string point_id = floor_child.attribute("start").value();
                    int point_index = get_point_by_id(point_id, floor_num);
                    paths[path_index].path_points.push_back(point_index);

                    read_path(floor_child, path_index, floor_num);
                }
                else {
                    process_xml_node(floor_child, -1, floor_num);
                }
            }
        }
    }
}
