#include "ray.h"
#include <math.h>

Ray::Ray() : o(Point(0, 0, 0)), d(Vector(0, 0, -1)){
}

Ray::Ray(const Point& origin, const Vector& direction) {
    o = origin;
    d = direction;
    d.normalize();
}

Ray::~Ray() {
}

bool Ray::operator==(const Ray& r2) const {
    return o == r2.origin() && d == r2.direction();
}

void Ray::set_origin(const Point& origin) {
    o = origin;
}
void Ray::set_direction(const Vector& direction) {
    d = direction;
    d.normalize();
}

Point Ray::origin() const {
    return o;
}

Vector Ray::direction() const {
    return d;
}

// -B +- sqrt(B^2 - 4C) / 2.0
// maybe just return the t value
float Ray::intersect_sphere(const Sphere& sphere) {
    Point center = sphere.center();
    float B = 2.0 * (d.x() * (o.x() - center.x()) + 
                    d.y() * (o.y() - center.y()) +
                    d.z() * (o.z() - center.z()));
    float C = pow(o.x() - center.x(), 2) + pow(o.y() - center.y(), 2) +
              pow(o.z() - center.z(), 2) - pow(sphere.radius(), 2);

    float discrim = (B * B) - (4 * C);
    if (discrim < 0) {
        return -1.0;
    }
    else if (discrim == 0) {
        return (-B / 2.0);
    }
    else  {
        float t1 = (-B + sqrt(discrim)) / 2.0;
        float t2 = (-B - sqrt(discrim)) / 2.0;
        
        if (t1 <= 0 && t2 > 0) {
            return t2;
        }
        else if (t1 > 0 && t2 <= 0) {
            return t1;
        }
        // regardless trace_ray ignores negative values
        else if (t1 <= 0 && t2 <= 0) {
            return t1;
        }
        // (t1 > 0 && t2 > 0)
        else {
            return std::min(t1, t2);
        }
    }
}

Point Ray::get_point(float t) {
    return o + (t * d);
}