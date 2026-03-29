#include <cmath>
#include "../include/math/ray.h"
#include "../include/gfx/scene.h"
#include "../include/math/constants.h"

Ray::Ray() : origin(Point3()), direction(Vector3()){}

Ray::Ray(Point3 origin, Vector3 direction) : origin(origin), direction(direction){
    entering = true;
    eta = Scene::bkgeta;
	o_primitive = nullptr;
    this->direction.normalize();
}

bool operator==(const Ray& r1, const Ray& r2) {
    return r1.origin == r2.origin && r1.direction == r2.direction;
}

Vector3 Ray::reflect(const Vector3& n) const {
    Vector3 v = direction;
    v = -v;
    Vector3 R = (2 * n.dot(v) * n) - v;
    R.normalize();
    return R;
}

Vector3 Ray::refract(Vector3 N, float ndotv, float n1, float n2) const{
    float snell = n1 / n2;
    Vector3 v(-direction.x, -direction.y, -direction.z);
    Vector3 B = snell * ((ndotv * N) - v);

    float discrim = 1.0f - (powf(snell, 2) * (1.0f - powf(ndotv, 2)));
    if (discrim < 0.0f) {
        // total internal reflections
        return Vector3(0.0f, 0.0f, 0.0f);
    }
    Vector3 A = sqrt(discrim) * (-N);
    return A + B;
}

Point3 Ray::get_point(float t) const {
    return origin + (t * direction);
}