#include "../include/geometry/primitive.h"
#include "../include/geometry/intersection.h"
#include "../include/geometry/shape.h"
#include "../include/math/floatutil.h"

Intersection Primitive::intersect(const Ray& r) {
    float t = geometry->intersect(r);
    if (is_negative(t)){
        return Intersection();
    }
    Point3 x_pos = r.get_point(t);
    Vector3 normal = geometry->get_normal(x_pos);
    return Intersection(r, t, this, x_pos, normal);
}