#include "../include/geometry/plane.h"
#include "../include/math/floatutil.h"
#include "../include/math/constants.h"

Plane::Plane() {
    pos = Point3();
    n = Vector3();
}

Plane::Plane(Point3 p, Vector3 n) : pos(p), n(n) {}

Hit Plane::intersect(const Ray& r) const {
    float denom = n.dot(r.direction);
    // if the ray is parallel to the plane
    if (is_near_zero(denom)) {
        return Hit();
    }
    float D = -n.dot(pos);
    float t = -(n.dot(r.origin) + D) / denom;
    if (is_negative(t)) {
        return Hit();
    }
    else {
        float mag = n.magnitude();
        Vector3 normal = (n.x * mag, n.y * mag, n.z * mag);

        return Hit(t, 0.0f, 0.0f, r, r.get_point(t), normal, nullptr);
    }
}