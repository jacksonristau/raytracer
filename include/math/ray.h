#pragma once

#include "vector3.h"
#include "point3.h"
#include <vector>

class Ray {
    public:
        Ray();
        Ray(Point3 origin, Vector3 direction);
        Ray(Point3 origin, Vector3 direction, bool entering, float eta, Primitive* p) :
            origin(origin),
            direction(direction),
            entering(entering),
            eta(eta),
            o_primitive(p) {
            direction.normalize();
        }

        // copy
        Ray(const Ray& r);

        // assignment
        Ray operator= (const Ray& r);

        virtual ~Ray();
        Vector3 reflect(const Vector3& normal) const;
        Vector3 refract(Vector3 normal, float ndotv, float n1, float n2) const;
        Hit intersect_scene_naive() const ;
        float intersect_scene_naive_shadow() const;

        Point3 get_point(float t) const;

        Point3 origin;
        Vector3 direction;
        bool entering;
        float eta;
        Primitive* o_primitive;
};

// equality
bool operator== (const Ray& r1, const Ray& r2);