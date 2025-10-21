#pragma once

#include <vector>
#include "shape.h"
#include "point3.h"
#include "point2.h"

class TriangleMesh {
    public:
        TriangleMesh();
        TriangleMesh(
            std::vector<Point3> vertices, 
            std::vector<Vector3> normals, 
            std::vector<Point2> uvs,
            std::vector<int> indices
        );

        int ntris, nvertices;

};