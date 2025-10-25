#include <cmath>
#include "../include/math/ray.h"
#include "../include/math/constants.h"

Ray::Ray() : origin(Point3()), direction(Vector3()){}

Ray::Ray(const Point3& origin, const Vector3& direction) : origin(origin), direction(direction){
    this->direction.normalize();
}

Ray::Ray(const Ray& r) {
    origin = r.origin;
    direction = r.direction;
}

Ray Ray::operator= (const Ray& r) {
    origin = r.origin;
    direction = r.direction;
    return *this;
}

Ray::~Ray() {}

bool operator==(const Ray& r1, const Ray& r2) {
    return r1.origin == r2.origin && r1.direction == r2.direction;
}

// check if t is negative
// float Ray::intersect_sphere(const Sphere& sphere) {
//     Point3 center = sphere.center;
//     float B = 2.0f * (d.x * (o.x - center.x) + 
//                      d.y * (o.y - center.y) +
//                      d.z * (o.z - center.z));
//     float C = std::pow(o.x - center.x, 2.0f) + std::pow(o.y - center.y, 2.0f) +
//               std::pow(o.z - center.z, 2.0f) - std::pow(sphere.radius, 2.0f);
//     float discrim = (B * B) - (4.0f * C);
//     if (discrim < 0.0f) {
//         return -1.0f;
//     }
//     else if (discrim == 0.0f) {
//         return (-B / 2.0f);
//     }
//     else  {
//         float t1 = (-B + sqrt(discrim)) / 2.0f;
//         float t2 = (-B - sqrt(discrim)) / 2.0f;
        
//         if (t1 < Eps && t2 > Eps) {
//             return t2;
//         }
//         else if (t1 > Eps && t2 < Eps) {
//             return t1;
//         }
//         // regardless trace_ray ignores negative values
//         else if (t1 < Eps && t2 <= Eps) {
//             return -1.0;
//         }
//         // (t1 > 0 && t2 > 0)
//         else {
//             return std::min(t1, t2);
//         }
//     }
// }

// check if t is negative if so its behind the ray origin
// float Ray::intersect_plane(const Vector3& normal, const Point3& point) {
//     float denom = normal.dot(direction);
//     // if the ray is parallel to the plane
//     /*if (denom > -0.0000001 && denom < 0.0000001) {
//         return -1.0;
//     }*/
//     if (fabs(denom) < 1e-6f) {
//         return -1.0f;
//     }
//     float D = -normal.dot(point);
//     float t = -(normal.dot(origin) + D) / denom;
//     if (t < Eps) return -1.0f;
//     return t;
// }

// coords expects a 3 element array
// float Ray::intersect_triangle(std::vector<Point3> vertices, float *coords) {
//     Vector3 e1 = vertices[1] - vertices[0];
//     Vector3 e2 = vertices[2] - vertices[0];
    
//     Vector3 n = e1.cross(e2);

//     float t = intersect_plane(n, vertices[0]);
//     if (t < Eps) {
//         return -1.0f;
//     }
//     // check if the point is inside the triangle
//     else {
//         float d11 = e1.dot(e1);
//         float d12 = e1.dot(e2);
//         float d22 = e2.dot(e2);
//         float det = (d11 * d22) - (d12 * d12);
//         /*if (det > -0.0001 && det < 0.0001) {
//             return -1.0;
//         }*/
//         if (fabs(det) < Eps) {
//             return -1.0f;
//         }
//         Point3 p = get_point(t);
//         Vector3 ep = p - vertices[0];
//         float dp1 = ep.dot(e1);
//         float dp2 = ep.dot(e2);

//         float beta = ((d22 * dp1) - (d12 * dp2)) / det;
//         float gamma = ((d11 * dp2) - (d12 * dp1)) / det;
//         float alpha = 1.0f - (beta + gamma);
//         if (alpha > Eps && alpha <= 1.0f && beta > Eps && beta <= 1.0f && gamma > Eps && gamma <= 1.0f) {
//             if (coords == NULL) {
//                 return t;
//             }
//             coords[0] = alpha;
//             coords[1] = beta;
//             coords[2] = gamma;
//             return t;
//         }
//         else {
//             return -1.0f;
//         }
//     }
// }

Vector3 Ray::reflect(const Vector3& N) {
    Vector3 I = -direction;
    Vector3 R = (2 * N.dot(I) * N) - I;
    R.normalize();
    return R;
}

Vector3 Ray::refract(Vector3 N, float n1, float n2) {
    Vector3 I = -direction;
    float snell = n1 / n2;
    float ndoti = N.dot(I);
    Vector3 B = snell * ((ndoti * N) - I);
    try{
        float discrim = 1.0f - (powf(snell, 2) * (1.0f - powf(ndoti, 2)));
        if (discrim < 0.0f) {
            throw std::exception();
        }
        Vector3 A = sqrt(discrim) * (-N);
        return A + B;
    }
    catch (std::exception e){
        // total internal reflection
        std::cout << "TIR" << std::endl;
        return Vector3(0.0f, 0.0f, 0.0f);
    }
}

Point3 Ray::get_point(float t) const {
    return origin + (t * direction);
}