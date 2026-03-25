#pragma once

#include "vector3.h"
#include "point3.h"
#include "ray.h"

class Matrix4x4 {
public:
    Matrix4x4();
    Matrix4x4(const float data[16]); 

    static Matrix4x4 identity();
    static Matrix4x4 translation(float x, float y, float z);
    static Matrix4x4 rotation_x(float degrees);
    static Matrix4x4 rotation_y(float degrees);
    static Matrix4x4 rotation_z(float degrees);
    static Matrix4x4 scale(float x, float y, float z);

    Matrix4x4 operator*(const Matrix4x4& other) const;
    Matrix4x4 transpose() const;

    Point3 transform_point(const Point3& p) const;
    Vector3 transform_vector(const Vector3& v) const; 
    Ray transform_ray(const Ray& r) const;
    bool is_identity() const;

    float operator()(int row, int col) const { return m[row][col]; }
    float& operator()(int row, int col) { return m[row][col]; }

private:
    // row major
    float m[4][4];
};
