#pragma once

#include "math/vector3.h"

class Sphere {
    public:
        Sphere();

        Sphere(const Point3& center, float radius);

        Sphere(const Point3& center, float radius, int m_index);

        virtual ~Sphere();

        // equality
        bool operator==(const Sphere& s1) const;

        // returns texture coordinates at a point on the sphere using polar coordinates
        int get_uv(const Point3& point, float* uv) const;

        Point3 c;
        float r;
        int m;
};