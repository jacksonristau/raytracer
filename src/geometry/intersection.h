#pragma once
#include "../primitive.h"
#include "../math/vector3.h"
#include "../math/point3.h"
#include "../math/ray.h"

// stores info about an intersection

class Intersection {
    public:
        Intersection();
        Intersection(Ray r, float t, Primitive o, Point3 p, Vector3 n);
        Intersection(const Intersection& i);
        ~Intersection();



        Ray x_ray;
        float t;
        Primitive* object;
        Point3 pos;
        Vector3 normal;
};