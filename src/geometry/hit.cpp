#include "../include/geometry/hit.h"

Hit::Hit() {
    r = Ray();
    x_pos = Point3();
    normal = Vector3();
    primitive = nullptr;
}

Hit Hit::infinity() {
    return Hit(INFINITY, 0.0f, 0.0f, Ray(), Point3(), Vector3(), nullptr);
}

Hit Hit::operator=(const Hit& h) {
    t = h.t,
    u = h.u;
    v = h.v;
    r = h.r;
    x_pos = h.x_pos;
    normal = h.normal;
    primitive = h.primitive;
    is_edge = h.is_edge;
    return *this;
}