#include "../include/geometry/sphere.h"
#include "../include/math/constants.h"
#include "../include/math/floatutil.h"


Sphere::Sphere() : center(Point3(0, 0, 0)), radius(1.0) {}

Sphere::Sphere(const Point3& center, float radius) : center(center), radius(radius) {}

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

Intersection Sphere::intersect(const Ray& r) const {
    float B = 2.0f * (r.direction.x * (r.origin.x - center.x) + 
                     r.direction.y * (r.origin.y - center.y) +
                     r.direction.z * (r.origin.z - center.z));
    float C = std::pow(r.origin.x - center.x, 2.0f) + 
             std::pow(r.origin.y - center.y, 2.0f) +
             std::pow(r.origin.z - center.z, 2.0f) - 
             std::pow(radius, 2.0f);
    float discrim = (B * B) - (4.0f * C);
    if (discrim < -Eps) {
        return Intersection();
    }
    else if (std::abs(discrim) < Eps) {
        float t = (-B / 2.0f);
        Point3 x_p = r.get_point(t);
        return Intersection(r, t, *this, x_p, this->normal(x_p));
    }
    else  {
        float t1 = (-B + sqrt(discrim)) / 2.0f;
        float t2 = (-B - sqrt(discrim)) / 2.0f;
        
        if (t1 < Eps && t2 > Eps) {
            return t2;
        }
        else if (t1 > Eps && t2 < Eps) {
            return t1;
        }
        // regardless trace_ray ignores negative values
        else if (t1 < Eps && t2 <= Eps) {
            return -1.0;
        }
        // (t1 > 0 && t2 > 0)
        else {
            return std::min(t1, t2);
        }
    }
}

// expects a point on the surface of the sphere
Vector3 Sphere::normal(const Point3& p) const {
    return p - center;
}

bool Sphere::operator==(const Sphere& s1) const {
    return center == s1.center && equalf(radius, s1.radius);
}