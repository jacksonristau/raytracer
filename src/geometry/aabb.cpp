#include "../../include/geometry/aabb.h"
#include "../../include/geometry/primitive.h"
#include "../../include/geometry/shape.h"
#include "../../include/geometry//triangle.h"

#include <limits>
#include <algorithm>

AABB::AABB(Point3 min, Point3 max) : min(min), max(max) {}

AABB::AABB(std::vector<Primitive*> primitives) {
    min = Point3::infinity();
    max = Point3::negative_infinity();
    for (Primitive* p : primitives) {
        AABB bounds = p->geometry->get_aabb();

        combine(bounds);
    }
}

AABB::AABB(std::vector<Primitive*>::const_iterator begin, std::vector<Primitive*>::const_iterator end) {
    min = Point3::infinity();
    max = Point3::negative_infinity();

    for (auto it = begin; it != end; ++it) {
        this->combine(expand(*(*it)));
    }
}

AABB AABB::expand(const Primitive& p) const {
    return this->combine(p.geometry->get_aabb());
}

AABB AABB::combine(AABB other) const {
    Point3 min = Point3::min(this->min, other.min);
    Point3 max = Point3::max(this->max, other.max);
    return AABB(min, max);
}

Point3 AABB::center() const{
    return Point3(0.5f * max.x + 0.5f * min.x, 0.5f * min.y + 0.5f * max.y, 0.5f * min.z + 0.5f * max.z);
}