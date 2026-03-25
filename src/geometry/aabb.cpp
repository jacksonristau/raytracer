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
    // find the max of the mins and the mins of the maxs to get the interval
    // if interval is valid and its max is greater than zero thats an intersection
    Vector3 min_distance = min - ray.origin;
    Vector3 max_distance = max - ray.origin;

    float t_min_x = min_distance.x / ray.direction.x;
    float t_min_y = min_distance.y / ray.direction.y;
    float t_min_z = min_distance.z / ray.direction.z;
    float t_max_x = max_distance.x / ray.direction.x;
    float t_max_y = max_distance.y / ray.direction.y;
    float t_max_z = max_distance.z / ray.direction.z;
    if (t_min_x > t_max_x) std::swap(t_min_x, t_max_x);
    if (t_min_y > t_max_y) std::swap(t_min_y, t_max_y);
    if (t_min_z > t_max_z) std::swap(t_min_z, t_max_z);

    Point2 overlap = Point2(std::max(std::max(t_min_x, t_min_y), t_min_z), std::min(std::min(t_max_x, t_max_y), t_max_z));

    // invalid interval
    if (overlap.y < overlap.x)
        return -1.0f;
    // behind origin
    if (overlap.y < 0)
        return -1.0f;

    return overlap.x;
}

Point3 AABB::center() const{
    return Point3(0.5f * max.x + 0.5f * min.x, 0.5f * min.y + 0.5f * max.y, 0.5f * min.z + 0.5f * max.z);
}