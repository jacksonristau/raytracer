#include "light.h"

Light::Light() : v(Vector(0, 0, 0)), i(0.0) {
    att[0] = 1.0;
    att[1] = 0.0;
    att[2] = 0.0;
}

Light::Light(Vector l, float intensity) : v(l), i(intensity) {
    att[0] = 1.0;
    att[1] = 0.0;
    att[2] = 0.0;
}

Light::~Light() {
}

void Light::set_att(float c1, float c2, float c3) {
    att[0] = c1;
    att[1] = c2;
    att[2] = c3;
}

float Light::atten(float d) const {
    return 1.0 / (att[0] + att[1] * d + att[2] * d * d);
}

std::ostream & operator<< ( std::ostream &os, const Light &l) {
  return os << l.l() << "intensity: " << l.intensity() <<  std::endl;
}