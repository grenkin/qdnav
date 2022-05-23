#!/usr/bin/python

import cgi
from plan import Plan

form = cgi.FieldStorage()

# print "The user entered %s" % form.getvalue("object")
object_param = form.getvalue("object")
from_param = form.getvalue("from")
to_param = form.getvalue("to")

plan1 = Plan("Plan_" + object_param + ".xml")
route = plan1.find_route(from_id, to_id)
route_text = plan1.process_route(route)

print "Content-type: text/html\n\n"
print "<html>Hello world!</html>"

for str in route_text:
    print("<p>" + str + "</p>")

print('<p><br/><a href="http://p98414p4.beget.tech/qdnav/">Вернуться</a></p>')