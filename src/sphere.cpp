#include "sphere.h"
#include "math/constants.h"
#include "math/floatutil.h"


Sphere::Sphere() : center(Point3(0, 0, 0)), radius(1.0), material_id(-1){}

Sphere::Sphere(const Point3& center, float radius, int m_index) : center(center), radius(radius), material_id(m_index) {}

Sphere::Sphere(const Point3& center, float radius) : center(center), radius(radius), material_id(-1){}

Sphere::~Sphere() {}

// expects a float array of size 2
int Sphere::get_uv(const Point3& point, float* uv) const {
    if (uv == NULL) {
        return 0;
    }
    Vector3 n = point - center;
    n = (1 / radius) * n;
    float theta = atan2(n.y, n.x);
    float phi = acos(n.z);
    uv[0] = std::max(theta / TwoPi, ((theta + TwoPi) / TwoPi));
    uv[1] = phi / Pi;
    return 1;
}

bool Sphere::operator==(const Sphere& s1) const {
    return center == s1.center && equalf(radius, s1.radius);
}