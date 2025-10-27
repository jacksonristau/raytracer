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

Hit Sphere::intersect(const Ray& r) const {
    Vector3 oc = r.origin - center;
    float B = 2.0f * r.direction.dot(oc);
    float C = oc.dot(oc) - (radius * radius);
    float discrim = (B * B) - (4.0f * C);

    if (is_negative(discrim)) {
        return Hit();
    }
    else if (is_near_zero(discrim)) {
        float t = (-B / 2.0f);
        Point3 x_p = r.get_point(t);
        return Hit(t, 0.0f, 0.0f, x_p, this->normal(x_p), nullptr);
    }
    else  {
        float sqrt_discrim = sqrt(discrim);
        float t1 = (-B + sqrt_discrim) / 2.0f;
        float t2 = (-B - sqrt_discrim) / 2.0f;
        
        if (is_negative(t1) && !is_negative(t2)) {
            Point3 x_p = r.get_point(t2);
            return Hit(t2, 0.0f, 0.0f, x_p, this->normal(x_p), nullptr);
        }
        else if (is_negative(t2) && !is_negative(t1)) {
            Point3 x_p = r.get_point(t1);
            return Hit(t1, 0.0f, 0.0f, x_p, this->normal(x_p), nullptr);
        }
        // regardless trace_ray ignores negative values
        else if (is_negative(t2) && is_negative(t1)) {
            return Hit();
        }
        // (t1 > 0 && t2 > 0)
        else {
            float mint = std::min(t1, t2);
            Point3 x_p = r.get_point(mint);
            return Hit(mint, 0.0f, 0.0f, x_p, this->normal(x_p), nullptr);
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