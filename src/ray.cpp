#include "ray.h"
#include <math.h>

Ray::Ray() : o(Vector(0, 0, 0)), d(Vector(0, 0, -1)){
}

Ray::Ray(const Vector& origin, const Vector& direction) {
    o = origin;
    d = direction;
    d.normalize();
}

Ray::~Ray() {
}

bool Ray::operator==(const Ray& r2) const {
    return o == r2.origin() && d == r2.direction();
}

void Ray::set_origin(const Vector& origin) {
    o = origin;
}
void Ray::set_direction(const Vector& direction) {
    d = direction;
    d.normalize();
}

Vector Ray::origin() const {
    return o;
}

Vector Ray::direction() const {
    return d;
}

// check if t is negative
float Ray::intersect_sphere(const Sphere& sphere) {
    Vector center = sphere.center();
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

// check if t is negative if so its behind the ray origin
float Ray::intersect_plane(const Vector& normal, const Vector& point) {
    float denom = normal.dot(d);
    // if the ray is parallel to the plane
    if (denom > -0.0000001 && denom < 0.0000001) {
        return -1.0;
    }
    float D = -normal.dot(point);
    return -(normal.dot(o) + D) / denom;
}

// coords expects a 3 element array
float Ray::intersect_triangle(std::vector<Vector> vertices, float *coords) {
    Vector e1 = vertices[1] - vertices[0];
    Vector e2 = vertices[2] - vertices[0];
    
    Vector n = e1.cross(e2);

    float t = intersect_plane(n, vertices[0]);
    if (t < 0.0) {
        return -1.0;
    }
    // check if the point is inside the triangle
    else {
        float d11 = e1.dot(e1);
        float d12 = e1.dot(e2);
        float d22 = e2.dot(e2);
        float det = (d11 * d22) - (d12 * d12);
        if (det > -0.0000001 && det < 0.0000001) {
            return -1.0;
        }
        Vector p = get_point(t);
        Vector ep = p - vertices[0];
        float dp1 = ep.dot(e1);
        float dp2 = ep.dot(e2);

        float beta = ((d22 * dp1) - (d12 * dp2)) / det;
        float gamma = ((d11 * dp2) - (d12 * dp1)) / det;
        float alpha = 1 - (beta + gamma);
        if (0 <= alpha && alpha <= 1 && 0 <= beta && beta <= 1 && 0 <= gamma && gamma <= 1) {
            if (coords == NULL) {
                return t;
            }
            coords[0] = alpha;
            coords[1] = beta;
            coords[2] = gamma;
            return t;
        }
        else {
            return -1.0;
        }
    }
}

Vector Ray::reflect(const Vector& N) {
    Vector I = -d;
    Vector R = (2 * N.dot(I) * N) - I;
    R.normalize();
    return R;
}

Vector Ray::get_point(float t) {
    return o + (t * d);
}