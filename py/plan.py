import xml.etree.ElementTree as et
from enum import Enum


class MotionDir(Enum):
    UP = 0
    RIGHT = 1
    DOWN = 2
    LEFT = 3


dir_names = ["up", "right", "down", "left"]


class Point:
    def __init__(self):
        self.id = ''
        self.name = ''
        self.hidden = False
        self.edges = []
        # если edge['path'] == -1, это лестница, если -2, то лифт


class Path:
    def __init__(self):
        self.path_points = []


class Plan:
    def add_point(self, point):
        self.points.append(point)
        point_index = len(self.points) - 1
        self.point_index_by_id[point.id] = point_index
        return point_index

    def add_point_to_path(self, point_index, path_index):
        self.paths[path_index].path_points.append(point_index)

    def get_stairs_point_id(self, point_id, floor):
        return point_id + "_stairs_" + str(floor)

    def get_elevator_point_id(self, point_id, floor):
        return point_id + "_elevator_" + str(floor)

    def get_point_by_id(self, point_id, floor):
        stairs_point_id = self.get_stairs_point_id(point_id, floor)
        elevator_point_id = self.get_elevator_point_id(point_id, floor)
        if stairs_point_id in self.point_index_by_id:
            point_index = self.point_index_by_id[stairs_point_id]
        elif elevator_point_id in self.point_index_by_id:
            point_index = self.point_index_by_id[elevator_point_id]
        else:
            point_index = self.point_index_by_id[point_id]
        return point_index

    def rotate_dir(self, dir, rotate_str):
        if rotate_str == 'forward':
            new_dir = dir
        elif rotate_str == 'left':
            new_dir = MotionDir((dir.value + 3) % 4)
        elif rotate_str == 'right':
            new_dir = MotionDir((dir.value + 1) % 4)
        elif rotate_str == 'back':
            new_dir = MotionDir((dir.value + 2) % 4)
        return new_dir

    def read_path(self, xml_node, path_index, floor_num):
        # xml_node.tag != 'Path'
        for node_child in xml_node:
            if node_child.tag == 'Path':
                path = Path()
                path.path_type = node_child.attrib['type']
                point_index = self.get_point_by_id(
                    node_child.attrib['start'], floor_num)
                path.path_points.append(point_index)
                rotate_str = node_child.attrib['rotate']
                parent_path_dir = self.paths[path_index].dir
                path.dir = self.rotate_dir(parent_path_dir, rotate_str)
                self.paths.append(path)
                new_path_index = len(self.paths) - 1

                self.read_path(node_child, new_path_index, floor_num)
            else:
                self.process_xml_node(node_child, path_index, floor_num)

    def process_xml_node(self, xml_node, path_index, floor_num):
        point = Point()
        point.floor = floor_num
        if 'id' in xml_node.attrib:
            point.id = xml_node.attrib['id']
        if 'name' in xml_node.attrib:
            point.name = xml_node.attrib['name']
        if 'location' in xml_node.attrib:
            point.location = xml_node.attrib['location']
        if 'show_name' in xml_node.attrib:
            point.hidden = (xml_node.attrib['show_name'] != '1')

        if point.id in self.point_index_by_id:
            # случай, когда точка с таким id уже есть
            point_index = self.get_point_by_id(point.id, floor_num)
        else:
            node_tag = xml_node.tag
            point.point_type = node_tag
            if node_tag == 'Point':
                point_index = self.add_point(point)
            elif node_tag == 'Room':
                if point.name == '':
                    point.name = point.id
                point_index = self.add_point(point)
            elif node_tag == 'Toilet':
                point_index = self.add_point(point)
            elif node_tag == 'Stairs':
                stairs_id = point.id
                stairs_point_id = self.get_stairs_point_id(stairs_id, floor_num)
                point.id = stairs_point_id
                point_index = self.add_point(point)
                if stairs_id not in self.stairs_by_id:
                    self.stairs_by_id[stairs_id] = []
                self.stairs_by_id[stairs_id].append(point_index)
            elif node_tag == 'Elevator':
                elevator_id = point.id
                elevator_point_id = self.get_elevator_point_id(elevator_id, floor_num)
                point.id = elevator_point_id
                point_index = self.add_point(point)
                if elevator_id not in self.elevators_by_id:
                    self.elevators_by_id[elevator_id] = []
                self.elevators_by_id[elevator_id].append(point_index)
            elif node_tag == 'Wall':
                point_index = self.add_point(point)
            elif node_tag == 'Door':
                point_index = self.add_point(point)
        if path_index != -1:
            self.paths[path_index].path_points.append(point_index)

    def read_plan(self, xml_file):
        tree = et.ElementTree(file=xml_file)
        root = tree.getroot()  # root.tag == 'Plan'
        for plan_child in root:
            if plan_child.tag == 'Exit':
                point_exit = Point()
                point_exit.floor = plan_child.attrib['floor']
                point_exit.id = plan_child.attrib['id']
                if 'name' in plan_child.attrib:
                    point_exit.name = plan_child.attrib['name']
                else:
                    point_exit.name = 'Выход'
                point_exit.point_type = 'Exit'
                self.add_point(point_exit)
            elif plan_child.tag == 'Floor':
                floor_num = int(plan_child.attrib['num'])
                for floor_child in plan_child:
                    if floor_child.tag == 'Path':
                        path = Path()
                        path.path_type = floor_child.attrib['type']
                        dir_str = floor_child.attrib['dir']
                        path.dir = MotionDir(dir_names.index(dir_str))
                        self.paths.append(path)
                        path_index = len(self.paths) - 1

                        # добавляем точку в путь
                        #point_id = floor_child.attrib['start']
                        point_index = self.get_point_by_id(
                            floor_child.attrib['start'], floor_num)
                        self.add_point_to_path(point_index, path_index)

                        self.read_path(floor_child, path_index, floor_num)
                    else:
                        self.process_xml_node(floor_child, -1, floor_num)

    def process_plan(self):
        for path_index in range(len(self.paths)):
            for i in range(len(self.paths[path_index].path_points)):
                if i == 0:
                    continue
                point_index = self.paths[path_index].path_points[i]
                prev_point_index = self.paths[path_index].path_points[i - 1]
                motion_dir = self.paths[path_index].dir
                edge1 = {'adj_point': point_index, 'path': path_index, 'reversed_path': False}
                self.points[prev_point_index].edges.append(edge1)
                edge2 = {'adj_point': prev_point_index, 'path': path_index, 'reversed_path': True}
                self.points[point_index].edges.append(edge2)

                # перебираем все лестницы и лифты, проводим ребра
                for stairs_id, stairs_list in self.stairs_by_id.items():
                    for i in range(len(stairs_list)):
                        for j in range(i+1, len(stairs_list)):
                            point_index = stairs_list[i]
                            edge1 = {'adj_point': stairs_list[j], 'path': -1, 'reversed_path': False}
                            self.points[point_index].edges.append(edge1)
                            point_index = stairs_list[j]
                            edge1 = {'adj_point': stairs_list[i], 'path': -1, 'reversed_path': False}
                            self.points[point_index].edges.append(edge2)

                for elevator_id, elevators_list in self.elevators_by_id.items():
                    for i in range(len(elevators_list)):
                        for j in range(i+1, len(elevators_list)):
                            point_index = elevators_list[i]
                            edge1 = {'adj_point': elevators_list[j], 'path': -2, 'reversed_path': False}
                            self.points[point_index].edges.append(edge1)
                            point_index = elevators_list[j]
                            edge1 = {'adj_point': elevators_list[i], 'path': -2, 'reversed_path': False}
                            self.points[point_index].edges.append(edge2)

    def __init__(self, xml_file):
        self.points = []
        self.paths = []
        self.point_index_by_id = {}
        self.stairs_by_id = {}
        self.elevators_by_id = {}

        self.read_plan(xml_file)
        self.process_plan()
