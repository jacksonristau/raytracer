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

        // equality
        bool operator==(const Vector3& v) const;

        // negation
        Vector3 operator-();

        float& operator[](int i) { return data[i]; }
        const float& operator[](int i) const { return data[i]; }

        bool is_zero();

        void normalize();

        Vector3 cross(const Vector3& v1) const;

        float magnitude() const;

        float distance(const Vector3& v1) const;

        inline float dot(const Vector3& v1) const {
            return (x * v1.x) + (y * v1.y) + (z * v1.z);
        }

        inline float dot(const Point3& p1) const {
            return (x * p1.x) + (y * p1.y) + (z * p1.z);
        }

        static const Vector3& Inf();

        union {
            float data[3];
            struct { float x, y, z; };
        };
};
Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3 operator*(float s, const Vector3& v1);

std::ostream & operator<< ( std::ostream &os, const Vector3 &v);