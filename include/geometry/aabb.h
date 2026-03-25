#pragma once
#include "../math/point3.h"
#include "mesh.h"
#include "primitive.h"
#include <vector>

class AABB {
    public:
        AABB() : min(Point3()), max(Point3()) {}
        AABB(Point3 min, Point3 max);
        AABB(std::vector<Primitive> primitives);
        AABB(std::vector<Primitive>::const_iterator begin,
            std::vector<Primitive>::const_iterator end);

        AABB expand(const Primitive& p) const;
        AABB combine(AABB bb) const;
        float intersect(const Ray& ray) const;

        Point3 center() const;
        Point3 min;
        Point3 max;
};