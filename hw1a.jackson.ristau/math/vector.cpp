#include "vector.h"
#include <math.h>
#include <iostream>

Vector::Vector() {
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
}

Vector::Vector(float x, float y, float z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

Vector::Vector(const Vector& v2) {
    v[0] = v2.x();
    v[1] = v2.y();
    v[2] = v2.z();
}

Vector::~Vector() {
}

bool Vector::operator==(const Vector& v2) const {
    return fabs(v[0] - v2.x()) < 0.001 && 
            fabs(v[1] - v2.y()) < 0.001 &&
            fabs(v[2] - v2.z()) < 0.001;
}

Vector Vector::operator-() {
    return Vector(v[0], -v[1], -v[2]);
}

Vector Vector::operator+(const Vector& v1) {
    return Vector(v[0] + v1.x(), v[1] + v1.y(), v[2] + v1.z());
}

Vector Vector::operator=(const Vector& v1) {
    v[0] = v1.x();
    v[1] = v1.y();
    v[2] = v1.z();
    return *this;
}

void Vector::normalize() {
    float magnitude = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] *= magnitude;
    v[1] *= magnitude;
    v[2] *= magnitude;
}

// vector1.cross(vector2) -> vector1 X vector2
Vector Vector::cross(const Vector& v1) {
    // v cross v1
    return Vector((y() * v1.z() - z() * v1.y()), 
                    (z() * v1.x() - x() * v1.z()), 
                    (x() * v1.y() - y() * v1.x()));
}

float Vector::dot(const Vector& v1) {
    return (x() * v1.x()) + (y() * v1.y()) + (z() * v1.z());
}

// addition
Point operator+(const Point& p1, const Vector& v1) {
    return Point(p1.x() + v1.x(), p1.y() + v1.y(), p1.z() + v1.z());
}

// subtraction
Point operator-(const Point& p1, const Vector& v1) {
    return Point(p1.x() - v1.x(), p1.y() - v1.y(), p1.z() - v1.z());
}

// p1 - p2 -> a vector pointing from p2 to p1
Vector operator-(const Point& p1, const Point& p2) {
    return Vector(p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z());
}

// scalar multiplication
Vector operator*(float s, const Vector& v1) {
    return Vector(s * v1.x(), s * v1.y(), s * v1.z());
}

std::ostream & operator<< ( std::ostream &os, const Vector &v) {
  return os << "<" << v.x() << ", " << v.y() << ", " << v.z() << ">";
}

