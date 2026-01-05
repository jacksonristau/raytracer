#include "../../include/geometry/boundingbox.h"

#include <limits>
#include <algorithm>

BoundingBox::BoundingBox(Mesh m) {
    auto vertices = m.get_vertices();
    if (vertices.size() > 0)
    {
        Point3 min_vertex = Point3(vertices[0], vertices[1], vertices[2]);
        Point3 max_vertex = Point3(vertices[0], vertices[1], vertices[2]);

        for (int i = 0; i < vertices.size(); i+=3)
        {
            min_vertex = Point3::min(min_vertex, Point3(vertices[i], vertices[i + 1], vertices[i + 2]));
            max_vertex = Point3::max(max_vertex, Point3(vertices[i], vertices[i + 1], vertices[i + 2]));
        }
        min = min_vertex;
        max = max_vertex;
        return;
    }
    min = Point3();
    max = Point3();
}

Point3 BoundingBox::center() const{
    return Point3(0.5f * max.x + 0.5f * min.x, 0.5f * min.y + 0.5f * max.y, 0.5f * min.z + 0.5f * max.z);
}