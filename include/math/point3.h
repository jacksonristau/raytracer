#pragma once

#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>

class Vector3;
using json = nlohmann::json;

class Point3 {
public:
    Point3();
    Point3(float x, float y, float z);
    Point3(json p);
    Point3(const Point3& p1);

    static Point3 max(const Point3& p1, const Point3& p2);
    static Point3 min(const Point3& p1, const Point3& p2);
    static Point3 infinity();
    static Point3 negative_infinity();

    virtual ~Point3();

    // negation
    Point3 operator- () const;

    // assignment
    Point3 operator= (const Point3& p1);

    // index access
    float& operator[](int i) { return data[i]; }
    const float& operator[](int i) const { return data[i]; }

    union {
        float data[3];
        struct { float x, y, z; };
    };
};

float distance(const Point3& p1, const Point3& p2);

Point3 operator+ (const Point3& p1, const Vector3& v1);

Point3 operator- (const Point3& p1, const Vector3& v1);

Vector3 operator- (const Point3& p1, const Point3& p2);

bool operator== (const Point3& p1, const Point3& p2);

Point3 operator* (float s, const Point3& p1);

Point3 operator* (const Point3& p1, float s);

std::ostream& operator<< (std::ostream& os, const Point3& p);