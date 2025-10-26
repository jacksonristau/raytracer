#include "../include/geometry/primitive.h"
#include "../include/geometry/hit.h"
#include "../include/geometry/intersection.h"
#include "../include/geometry/shape.h"
#include "../include/math/floatutil.h"

Intersection Primitive::intersect(const Ray& r) {
    Hit h;
    if (!geometry->intersect(r, h))
        return Intersection();

    Point3 x_pos = r.get_point(h.t);
    Vector3 normal = geometry->normal(h, x_pos);
    return Intersection(r, h.t, this, x_pos, normal);
}