#include "sphere.h"
#include "math/constants.h"
#include "math/floatutil.h"


Sphere::Sphere() : c(Point3(0, 0, 0)), r(1.0), m(-1){}

Sphere::Sphere(const Point3& center, float radius, int m_index) : c(center), r(radius), m(m_index) {} 

Sphere::Sphere(const Point3& center, float radius) : c(center), r(radius), m(-1){}

Sphere::~Sphere() {}

// expects a float array of size 2
int Sphere::get_uv(const Point3& point, float* uv) const {
    if (uv == NULL) {
        return 0;
    }
    Vector3 n = point - c;
    n = (1 / r) * n;
    float theta = atan2(n.y, n.x);
    float phi = acos(n.z);
    uv[0] = std::max(theta / TwoPi, ((theta + TwoPi) / TwoPi));
    uv[1] = phi / Pi;
    return 1;
}

bool Sphere::operator==(const Sphere& s1) const {
    return c == s1.c && equalf(r, s1.r);
}