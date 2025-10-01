#include "lightd.h"

LightD::LightD() {
    l = Vector3();
    i = 0.0f;
    c0 = 1.0f;
    c1 = 0.0f;
    c2 = 0.0f;
}

LightD::LightD(Vector3 l, float i) : l(l), i(i) {
    c0 = 1.0f;
    c1 = 0.0f;
    c2 = 0.0f;
}

LightD::LightD(Vector3 l, float i, float c0, float c1, float c2) : l(l), i(i), c0(c0), c1(c1), c2(c2){
    l.normalize();
}

LightD::~LightD() {}

float LightD::atten(float d) const {
    return 1.0f / (c0 + c1 * d + c2 * d * d);
}

Ray LightD::get_shadow_ray(const Point3& x_p) const {
    return Ray(x_p, l);
}

void LightD::print(std::ostream& os) const {
    os << "directional light: <" << l.x << ", " << l.y << ", " << l.z << ">, intensity: " << i;
}

bool LightD::is_point() const {
    return false;
}