#include "../../include/geometry/aabb.h"
#include "../../include/geometry/primitive.h"
#include "../../include/geometry/shape.h"
#include "../../include/geometry//triangle.h"

#include <limits>
#include <algorithm>

AABB::AABB(Point3 min, Point3 max) : min(min), max(max) {}

AABB::AABB(std::vector<Primitive> primitives) {
    min = Point3::infinity();
    max = Point3::negative_infinity();
    for (Primitive p : primitives) {
        AABB bounds = p.get_aabb();

        *this = combine(bounds);
    }
}

AABB::AABB(std::vector<Primitive>::const_iterator begin, std::vector<Primitive>::const_iterator end) {
    min = Point3::infinity();
    max = Point3::negative_infinity();

    for (auto it = begin; it != end; ++it) {
        *this = this->combine(expand((*it)));
    }
}

AABB AABB::expand(const Primitive& p) const {
    return this->combine(p.get_aabb());
}

AABB AABB::combine(AABB other) const {
    Point3 min = Point3::min(this->min, other.min);
    Point3 max = Point3::max(this->max, other.max);
    return AABB(min, max);
}

float AABB::intersect(const Ray& ray) const {
    // find t_min and t_max for each plane
    // swap min and max if necessary
    // find the max of the mins and the mins of the maxs to get the interval
    // if interval is valid and its max is greater than zero thats an intersection
    const float inv_dx = 1.0f / ray.direction.x;
    const float inv_dy = 1.0f / ray.direction.y;
    const float inv_dz = 1.0f / ray.direction.z;

    const float t1x = (min.x - ray.origin.x) * inv_dx;
    const float t1y = (min.y - ray.origin.y) * inv_dy;
    const float t1z = (min.z - ray.origin.z) * inv_dz;

    const float t2x = (max.x - ray.origin.x) * inv_dx;
    const float t2y = (max.y - ray.origin.y) * inv_dy;
    const float t2z = (max.z - ray.origin.z) * inv_dz;
    
    const float t_min_x = fminf(t1x, t2x);
    const float t_min_y = fminf(t1y, t2y);
    const float t_min_z = fminf(t1z, t2z);

    const float t_max_x = fmaxf(t1x, t2x);
    const float t_max_y = fmaxf(t1y, t2y);
    const float t_max_z = fmaxf(t1z, t2z);

    const float t_min = fmaxf(fmaxf(t_min_x, t_min_y), t_min_z);
    const float t_max = fminf(fminf(t_max_x, t_max_y), t_max_z);

    // invalid interval
    if (t_max < t_min)
        return -1.0f;
    // behind origin
    if (t_max < 0.0f)
        return -1.0f;

    return t_min;
}

Point3 AABB::center() const{
    return Point3(0.5f * max.x + 0.5f * min.x, 0.5f * min.y + 0.5f * max.y, 0.5f * min.z + 0.5f * max.z);
}