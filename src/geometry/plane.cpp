#include "../include/geometry/plane.h"
#include "../include/math/floatutil.h"
#include "../include/math/constants.h"

Plane::Plane() {
    pos = Point3();
    normal = Vector3();
}

Plane::Plane(Point3 p, Vector3 n) : pos(p), normal(n) {}

float Plane::intersect(const Ray& r) const {
    float denom = normal.dot(r.direction);
    // if the ray is parallel to the plane
    if (is_near_zero(denom)) {
        return -1.0f;
    }
    float D = -normal.dot(pos);
    float t = -(normal.dot(r.origin) + D) / denom;
    if (t < Eps) return -1.0f;
    return t;
}