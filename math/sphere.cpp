#include "sphere.h"
#include <math.h>

Sphere::Sphere() : c(Point(0, 0, 0)), r(1.0){
}

Sphere::Sphere(const Point& center, float radius) : c(center), r(radius) {
}

Sphere::~Sphere() {
}

bool Sphere::operator==(const Sphere& s2) const {
    return c == s2.center() && r == s2.radius();
}

void Sphere::set(const Point& center, float radius) {
    c = center;
    r = radius;
}