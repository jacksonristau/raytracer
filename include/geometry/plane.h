#pragma once
#include "../include/math/point3.h"
#include "../include/math/vector3.h"
#include "shape.h"
#include "hit.h"

/*
    plane eq: A*x + B*y + C*z + D
    A B C = n.x, n.y, n.z
    
    a plane is a point p1 and a normal n for all points p where (p - p1) is perpendicular to n
    ie. (p - p1) dot n = 0
*/

class Plane : public IShape {
    public:
        Plane();
        Plane(Point3 p, Vector3 n);

        int get_type() const override {return 2;}
        Hit intersect(const Ray& r, bool with_uv = false) const override;
        Vector3 normal(const Hit& h, const Point3& p) const override { return n; }

        Point3 pos;
        Vector3 n;
};