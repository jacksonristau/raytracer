#ifndef RAY_H
#define RAY_H

#include "vector.h"
#include "sphere.h"

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

        Vector get_point(float t);

        Vector origin() const;
        Vector direction() const;

    private:
        Vector o;
        Vector d;
};
#endif