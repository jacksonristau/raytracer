#pragma once

#include "vector3.h"
#include "point3.h"
#include <vector>

class Ray {
    public:
        Ray();
        Ray(const Point3& origin, const Vector3& direction);

        // copy
        Ray(const Ray& r);

        // assignment
        Ray operator= (const Ray& r);

        virtual ~Ray();
        //float intersect_plane(const Vector3& normal, const Point3& point);
        // returns fills the array coords with the barycentric coordinates of the intersection
        //float intersect_triangle(std::vector<Point3> vertices, float* coords);
        Vector3 reflect(const Vector3& normal);
        Vector3 refract(Vector3 normal, float n1, float n2);

        Point3 get_point(float t) const;

        Point3 origin;
        Vector3 direction;
};

// equality
bool operator== (const Ray& r1, const Ray& r2);