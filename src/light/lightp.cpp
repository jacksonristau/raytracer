#include "lightp.h"

LightP::LightP() {
    l = Point3();
    i = 0.0f;
    c0 = 1.0f;
    c1 = 0.0f;
    c2 = 0.0f;
}

LightP::LightP(Point3 l, float i) : l(l), i(i) {
    c0 = 1.0f;
    c1 = 0.0f;
    c2 = 0.0f;
}

LightP::LightP(Point3 l, float i, float c0, float c1, float c2) : l(l), i(i), c0(c0), c1(c1), c2(c2){}

LightP::~LightP() {}

float LightP::atten(float d) const {
    return 1.0f / (c0 + c1 * d + c2 * d * d);
}

Ray LightP::get_shadow_ray(const Point3& x_p) const {
    return Ray(x_p, l - x_p);
}

void LightP::print(std::ostream& os) const {
    os << "point light: (" << l.x << ", " << l.y << ", " << l.z << "), i: " << i;
}

bool LightP::is_point() const {
    return true;
}

float LightP::dist(const Point3& p) const {
    return distance(p, l);
}