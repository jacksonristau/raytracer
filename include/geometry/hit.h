#pragma once
#include "../math/floatutil.h"
#include "../math/ray.h"

class Primitive;
class Ray;

class Hit {
    public: 
        Hit();
        Hit(float t, float u, float v, Ray r, Point3 p, Vector3 n, Primitive* prim) :
            t(t),
            u(u),
            v(v),
            r(r),
            x_pos(p),
            normal(n),
            primitive(prim) {}
        
        static Hit infinity();

        float t = -1.0f;
        float u = 0.0f;
        float v = 0.0f;

        Ray r;
        Point3 x_pos;
        Vector3 normal;
        Primitive* primitive;
        bool is_edge = false;

        Hit operator=(const Hit& h);

        bool valid() const { return ! is_negative(t) && ! std::isinf(t); }
};