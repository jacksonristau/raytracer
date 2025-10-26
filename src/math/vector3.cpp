#include <iostream>

#include "../include/math/vector3.h"
#include "../include/math/floatutil.h"

static const Vector3 INF = Vector3(INFINITY, INFINITY, INFINITY);
const Vector3& Vector3::Inf() {
    return INF;
}

static const Vector3 ZERO = Vector3(0.0f, 0.0f, 0.0f);

// defaults to a Vector3
Vector3::Vector3(): x(0.0f), y(0.0f), z(0.0f) {
}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z){}
Vector3::Vector3(json v) {
    x = v[0];
    y = v[1];
    z = v[2];
}

Vector3::Vector3(const Vector3& v2) {
    x = v2.x;
    y = v2.y;
    z = v2.z;
}

Vector3::~Vector3() {
}

bool Vector3::operator==(const Vector3& v1) const {
    return equalf(x, v1.x) &&
           equalf(y, v1.y) &&
           equalf(z, v1.z);
}

Vector3 Vector3::operator-() {
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator=(const Vector3& v1) {
    x = v1.x;
    y = v1.y;
    z = v1.z;
    return *this;
}

void Vector3::normalize() {
    float magnitude = 1.0f / sqrt(x * x + y * y + z * z);
    x *= magnitude;
    y *= magnitude;
    z *= magnitude;
}

// v1.cross(v2) -> v1 X v2
Vector3 Vector3::cross(const Vector3& v1) const {
    // v cross v1
    return Vector3((y * v1.z - z * v1.y), 
                    (z * v1.x - x * v1.z), 
                    (x * v1.y - y * v1.x));
}

float Vector3::distance(const Vector3& v1) const {
    return sqrt(
        (x - v1.x) * (x - v1.x) +
        (y - v1.y) * (y - v1.y) + 
        (z - v1.z) * (z - v1.z)
    );
}

float Vector3::dot(const Vector3& v1) const {
    return (x * v1.x) + (y * v1.y) + (z * v1.z);
}

float Vector3::dot(const Point3& p1) const {
    return (x * p1.x) + (y * p1.y) + (z * p1.z);
}

// good practice to check if w is not 1 or 0
Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// scalar multiplication
Vector3 operator*(float s, const Vector3& v1) {
    return Vector3(s * v1.x, s * v1.y, s * v1.z);
}

std::ostream & operator<< ( std::ostream &os, const Vector3 &v) {
  return os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
}

