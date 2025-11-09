#include "../include/geometry/sphere.h"
#include "../include/math/constants.h"
#include "../include/math/floatutil.h"
#include "../include/geometry/hit.h"
#include "../include/math/point2.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;


Sphere::Sphere() : center(Point3(0, 0, 0)), radius(1.0) {}

Sphere::Sphere(const Point3& center, float radius) : center(center), radius(radius) {}

Sphere::Sphere(json sphere_json) {
    center = Point3(sphere_json.at("center"));
    radius = sphere_json.at("radius");
}

Sphere::~Sphere() {}

Point2 Sphere::get_uv(const Point3& point) const {
    Vector3 n = point - center;
    n = (1 / radius) * n;
    float theta = atan2(n.y, n.x);
    float phi = acos(n.z);
    float u = std::max(theta / TwoPi, ((theta + TwoPi) / TwoPi));
    float v = phi / Pi;
    return Point2(u, v);
}

Hit Sphere::intersect(const Ray& r, bool with_uv) const {
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
        Point2 uv = with_uv ? get_uv(x_p) : Point2(0.0f, 0.0f);
        return Hit(t, uv.x, uv.y, r, x_p, Vector3(), nullptr);
    }
    else  {
        float sqrt_discrim = sqrt(discrim);
        float t1 = (-B + sqrt_discrim) / 2.0f;
        float t2 = (-B - sqrt_discrim) / 2.0f;
        
        if (is_negative(t1) && !is_negative(t2)) {
            Point3 x_p = r.get_point(t2);
            Point2 uv = with_uv ? get_uv(x_p) : Point2(0.0f, 0.0f);
            return Hit(t2, uv.x, uv.y, r, x_p, this->normal(Hit(), x_p), nullptr);
        }
        else if (is_negative(t2) && !is_negative(t1)) {
            Point3 x_p = r.get_point(t1);
            Point2 uv = with_uv ? get_uv(x_p) : Point2(0.0f, 0.0f);
            return Hit(t1, uv.x, uv.y, r, x_p, this->normal(Hit(), x_p), nullptr);
        }
        // regardless trace_ray ignores negative values
        else if (is_negative(t2) && is_negative(t1)) {
            return Hit();
        }
        // (t1 > 0 && t2 > 0)
        else {
            float min_t = std::min(t1, t2);
            Point3 x_p = r.get_point(min_t);
            Point2 uv = with_uv ? get_uv(x_p) : Point2(0.0f, 0.0f);
            return Hit(min_t, uv.x, uv.y, r, x_p, this->normal(Hit(), x_p), nullptr);
        }
    }
}

// expects a point on the surface of the sphere
Vector3 Sphere::normal(const Hit& h, const Point3& p) const {
    Vector3 v = p - center;
    v.normalize();
    return v;
}

bool Sphere::operator==(const Sphere& s1) const {
    return center == s1.center && equalf(radius, s1.radius);
}