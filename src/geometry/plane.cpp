#include "../include/geometry/plane.h"
#include "../include/math/floatutil.h"
#include "../include/math/constants.h"

Plane::Plane(Point3 p, Vector3 n) : pos(p), n(n) {}

Hit Plane::intersect(const Ray& r, bool with_uv) const {
    float denom = n.dot(r.direction);
    // if the ray is parallel to the plane
    if (denom == 0.0f)
        return Hit();

    float D = -n.dot(pos);
    float t = -(n.dot(r.origin) + D) / denom;
    if (is_negative(t)) {
        return Hit();
    }
    else {

        return Hit(t, 0.0f, 0.0f, r, r.get_point(t), n, nullptr);
    }
}