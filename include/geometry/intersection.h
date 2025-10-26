#pragma once

#include "primitive.h"
#include "../math/vector3.h"
#include "../math/point3.h"
#include "../math/ray.h"

// stores info about an intersection

class Intersection {
    public:
        Intersection() : x_ray(Ray()), t(-1.0f), object(nullptr), pos(Point3()), normal(Vector3()) {}
        Intersection(Ray r, float t, Primitive* o, Point3 p, Vector3 n) :  x_ray(r), t(t), object(o), pos(p), normal(n) {}

        Intersection(const Intersection& i) {
            x_ray = i.x_ray;
            t = i.t;
            object = i.object;
            pos = i.pos;
            normal = i.normal;
        }

        ~Intersection() {}

        Ray x_ray;
        float t;
        Primitive* object;
        Point3 pos;
        Vector3 normal;
};