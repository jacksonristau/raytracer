#include "../include/geometry/primitive.h"
#include "../include/geometry/hit.h"
#include "../include/geometry/shape.h"
#include "../include/math/floatutil.h"

Hit Primitive::intersect(const Ray& r) {
    Hit h = geometry->intersect(r);
    h.primitive = this;
    return h;
}