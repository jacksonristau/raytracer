#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector.h"
#include "sphere.h"

class Ray {
    public:
        Ray();

        Ray(const Point& origin, const Vector& direction);

        virtual ~Ray();

        // equality
        bool operator==(const Ray& r2) const;

        void set_origin(const Point& origin);
        void set_direction(const Vector& direction);

        float intersect_sphere(const Sphere& sphere);

        Point get_point(float t);

        Point origin() const;
        Vector direction() const;

    private:
        Point o;
        Vector d;
};
#endif