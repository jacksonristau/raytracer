#include "../include/geometry/primitive.h"
#include "../include/geometry/hit.h"
#include "../include/geometry/shape.h"
#include "../include/math/floatutil.h"
#include "../include/gfx/material.h"

Hit Primitive::intersect(const Ray& r) {
    bool with_uv = !material->is_uniform();

    Hit h = geometry->intersect(r, with_uv);
    h.primitive = this;
    return h;
}