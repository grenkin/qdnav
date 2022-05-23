from plan import Plan


plan1 = Plan("Plan.xml")

#for p in plan1.paths:
#    print(p.path_points)

from_id = "2"
to_id = "1308"
route = plan1.find_route(from_id, to_id)
#print(route)
route_text = plan1.process_route(route)
for str in route_text:
    print(str)
