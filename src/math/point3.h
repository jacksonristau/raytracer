#pragma once
#include "vector3.h"
#include <iostream>

class Point3 {
public:
    Point3();

    Point3(float x, float y, float z);

    Point3(const Point3& p1);

    virtual ~Point3();

    // negation
    Point3 operator- () const;

    // subtraction
    Vector3 operator- (const Point3& p1) const;

    // assignment
    Point3 operator= (const Point3& p1);

    float x, y, z;
};

float distance(const Point3& p1, const Point3& p2);

Point3 operator+ (Point3& p1, Vector3& v1);

bool operator== (const Point3& p1, const Point3& p2);

Point3 operator* (float s, const Point3& p1);

Point3 operator* (const Point3& p1, float s);

std::ostream& operator<< (std::ostream& os, const Point3& p);