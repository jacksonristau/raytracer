#ifndef RAY_H
#define RAY_H

#include "vector.h"
#include "sphere.h"
#include <vector>

class Ray {
    public:
        Ray();

        Ray(const Vector& origin, const Vector& direction);

        virtual ~Ray();

        // equality
        bool operator==(const Ray& r2) const;

        void set_origin(const Vector& origin);
        void set_direction(const Vector& direction);

        float intersect_sphere(const Sphere& sphere);
        float intersect_plane(const Vector& normal, const Vector& point);
        // returns fills the array coords with the barycentric coordinates of the intersection
        float intersect_triangle(std::vector<Vector> vertices, float* coords);
        Vector reflect(const Vector& normal);
        Vector refract(Vector normal, float n1, float n2);

        Vector get_point(float t);

        Vector origin() const;
        Vector direction() const;

    private:
        Vector o;
        Vector d;
};
#endif