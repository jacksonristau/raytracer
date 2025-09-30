#pragma once

#include "vector3.h"
#include "point3.h"
#include "sphere.h"
#include <vector>

class Ray {
    public:
        Ray();

        Ray(const Point3& origin, const Vector3& direction);

        virtual ~Ray();

        void set_origin(const Point3& origin);
        void set_direction(const Vector3& direction);

        float intersect_sphere(const Sphere& sphere);
        float intersect_plane(const Vector3& normal, const Point3& point);
        // returns fills the array coords with the barycentric coordinates of the intersection
        float intersect_triangle(std::vector<Point3> vertices, float* coords);
        Vector3 reflect(const Vector3& normal);
        Vector3 refract(Vector3 normal, float n1, float n2);

        Point3 get_point(float t);

        Point3 o;
        Vector3 d;
};

// equality
bool operator== (const Ray& r1, const Ray& r2);