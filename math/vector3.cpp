#include <vector3.h>
#include <math.h>

Vector3::Vector3() {
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
}

Vector3::Vector3(float x, float y, float z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

Vector3::Vector3(const Vector3& v2) {
    v[0] = v2.x();
    v[1] = v2.y();
    v[2] = v2.z();
}

Vector3::~Vector3() {
}

bool Vector3::operator==(const Vector3& v2) const {
    return fabs(v[0] - v2.x()) < 0.001 && 
            fabs(v[1] - v2.y()) < 0.001 &&
            fabs(v[2] - v2.z()) < 0.001;
}

Vector3 Vector3::operator-(const Vector3& v) {
    return Vector3(-v.x(), -v.y(), -v.z());
}

Vector3 Vector3::operator+(const Vector3& v1) {
    return Vector3(v[0] + v1.x(), v[1] + v1.y(), v[2] + v1.z());
}

Vector3 Vector3::operator=(const Vector3& v1) {
    v[0] = v1.x();
    v[1] = v1.y();
    v[2] = v1.z();
    return *this;
}