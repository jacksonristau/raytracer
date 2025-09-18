#include "sphere.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI


Sphere::Sphere() : c(Vector(0, 0, 0)), r(1.0){
}

Sphere::Sphere(const Vector& center, float radius, int m_index) : c(center), r(radius), m(m_index) {
} 

Sphere::Sphere(const Vector& center, float radius) : c(center), r(radius) {
}

Sphere::~Sphere() {
}

// expects a float array of size 2
int Sphere::get_uv(const Vector& point, float* uv) const {
    if (uv == NULL) {
        return 0;
    }
    Vector n = point - c;
    n = (1 / r) * n;
    float theta = atan2(n.y(), n.x());
    float phi = acos(n.z());
    uv[0] = std::max(theta / (2 * M_PI), ((theta + (2 * M_PI)) / (2 * M_PI)));
    uv[1] = phi / (M_PI);
    return 1;
}

bool Sphere::operator==(const Sphere& s2) const {
    return c == s2.center() && r == s2.radius();
}

void Sphere::set(const Vector& center, float radius) {
    c = center;
    r = radius;
}