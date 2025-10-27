#pragma once
#include "../math/floatutil.h"
#include "primitive.h"

class Hit {
    public: 
        Hit();
        Hit(float t, float u, float v, Point3 p, Vector3 n, Primitive* prim);
        float t = -1.0f;
        float u = 0.0f;
        float v = 0.0f;

        Point3 x_pos;
        Vector3 normal;
        Primitive* primitive;

        bool valid() const { return ! is_negative(t); }
};