#include "ray.h"
#include <cmath>

const float EPSILON = 1e-8f;

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
    float B = 2.0f * (d.x() * (o.x() - center.x()) + 
                     d.y() * (o.y() - center.y()) +
                     d.z() * (o.z() - center.z()));
    float C = std::pow(o.x() - center.x(), 2.0f) + std::pow(o.y() - center.y(), 2.0f) +
              std::pow(o.z() - center.z(), 2.0f) - std::pow(sphere.radius(), 2.0f);
    float discrim = (B * B) - (4.0f * C);
    if (discrim < 0.0f) {
        return -1.0f;
    }
    else if (discrim == 0.0f) {
        return (-B / 2.0f);
    }
    else  {
        float t1 = (-B + sqrt(discrim)) / 2.0f;
        float t2 = (-B - sqrt(discrim)) / 2.0f;
        
        if (t1 < EPSILON && t2 > EPSILON) {
            return t2;
        }
        else if (t1 > EPSILON && t2 < EPSILON) {
            return t1;
        }
        // regardless trace_ray ignores negative values
        else if (t1 < EPSILON && t2 <= EPSILON) {
            return -1.0;
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
    /*if (denom > -0.0000001 && denom < 0.0000001) {
        return -1.0;
    }*/
    if (fabs(denom) < 1e-6f) {
        return -1.0f;
    }
    float D = -normal.dot(point);
    float t = -(normal.dot(o) + D) / denom;
    if (t < 1e-6f) return -1.0f;
    return t;
}

// coords expects a 3 element array
float Ray::intersect_triangle(std::vector<Vector> vertices, float *coords) {
    Vector e1 = vertices[1] - vertices[0];
    Vector e2 = vertices[2] - vertices[0];
    
    Vector n = e1.cross(e2);

    float t = intersect_plane(n, vertices[0]);
    if (t < 1e-6f) {
        return -1.0f;
    }
    // check if the point is inside the triangle
    else {
        float d11 = e1.dot(e1);
        float d12 = e1.dot(e2);
        float d22 = e2.dot(e2);
        float det = (d11 * d22) - (d12 * d12);
        /*if (det > -0.0001 && det < 0.0001) {
            return -1.0;
        }*/
        if (fabs(det) < EPSILON) {
            return -1.0f;
        }
        Vector p = get_point(t);
        Vector ep = p - vertices[0];
        float dp1 = ep.dot(e1);
        float dp2 = ep.dot(e2);

        float beta = ((d22 * dp1) - (d12 * dp2)) / det;
        float gamma = ((d11 * dp2) - (d12 * dp1)) / det;
        float alpha = 1.0f - (beta + gamma);
        const float BARY_EPS = 1e-6f;
        if (alpha >= -BARY_EPS && alpha <= 1.0f && beta >= -BARY_EPS && beta <= 1.0f && gamma >= -BARY_EPS && gamma <= 1.0f) {
            if (coords == NULL) {
                return t;
            }
            coords[0] = alpha;
            coords[1] = beta;
            coords[2] = gamma;
            return t;
        }
        else {
            return -1.0f;
        }
    }
}

Vector Ray::reflect(const Vector& N) {
    Vector I = -d;
    Vector R = (2 * N.dot(I) * N) - I;
    R.normalize();
    return R;
}

Vector Ray::refract(Vector N, float n1, float n2) {
    Vector I = -d;
    float snell = n1 / n2;
    float ndoti = N.dot(I);
    Vector B = snell * ((ndoti * N) - I);
    try{
        float discrim = 1 - (powf(snell, 2) * (1.0f - powf(ndoti, 2)));
        if (discrim < 0) {
            throw std::exception();
        }
        Vector A = sqrt(discrim) * (-N);
        return A + B;
    }
    catch (std::exception e){
        std::cout << "tir" << std::endl;
        return Vector(0.0f, 0.0f, 0.0f);
    }
}

Vector Ray::get_point(float t) {
    return o + (t * d);
}