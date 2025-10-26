#pragma once

#include <iostream>
#include "point3.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Vector3 {
    public:
        Vector3();

        Vector3(float x, float y, float z);
        Vector3(json v);

        Vector3(const Vector3& v2);

        virtual ~Vector3();

        // equality
        bool operator==(const Vector3& v) const;

        // negation
        Vector3 operator-();

        // assignment
        Vector3 operator=(const Vector3& v1);

        void normalize();

        Vector3 cross(const Vector3& v1) const;

        float distance(const Vector3& v1) const;

        float dot(const Vector3& v1) const;

        float dot(const Point3& p1) const;

        static const Vector3& Inf();

        float x, y, z;
};
Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3 operator*(float s, const Vector3& v1);

std::ostream & operator<< ( std::ostream &os, const Vector3 &v);