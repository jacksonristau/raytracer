#include "../include/math/matrix4x4.h"
#include "../include/math/constants.h"
#include <cmath>
#include <cstring>

// Constructors
Matrix4x4::Matrix4x4() {
    // Initialize to identity
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

Matrix4x4::Matrix4x4(const float data[16]) {
    // Copy from row-major array
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = data[i * 4 + j];
        }
    }
}

// Factory methods
Matrix4x4 Matrix4x4::identity() {
    return Matrix4x4();
}

Matrix4x4 Matrix4x4::translation(float x, float y, float z) {
    Matrix4x4 result;
    result.m[0][3] = x;
    result.m[1][3] = y;
    result.m[2][3] = z;
    return result;
}

Matrix4x4 Matrix4x4::rotation_x(float degrees) {
    float radians = degrees * Pi / 180.0f;
    float c = std::cos(radians);
    float s = std::sin(radians);

    Matrix4x4 result;
    result.m[1][1] = c;
    result.m[1][2] = -s;
    result.m[2][1] = s;
    result.m[2][2] = c;
    return result;
}

Matrix4x4 Matrix4x4::rotation_y(float degrees) {
    float radians = degrees * Pi / 180.0f;
    float c = std::cos(radians);
    float s = std::sin(radians);

    Matrix4x4 result;
    result.m[0][0] = c;
    result.m[0][2] = s;
    result.m[2][0] = -s;
    result.m[2][2] = c;
    return result;
}

Matrix4x4 Matrix4x4::rotation_z(float degrees) {
    float radians = degrees * Pi / 180.0f;
    float c = std::cos(radians);
    float s = std::sin(radians);

    Matrix4x4 result;
    result.m[0][0] = c;
    result.m[0][1] = -s;
    result.m[1][0] = s;
    result.m[1][1] = c;
    return result;
}

Matrix4x4 Matrix4x4::scale(float x, float y, float z) {
    Matrix4x4 result;
    result.m[0][0] = x;
    result.m[1][1] = y;
    result.m[2][2] = z;
    return result;
}

// Matrix operations
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
    Matrix4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Matrix4x4 Matrix4x4::transpose() const {
    Matrix4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = m[j][i];
        }
    }
    return result;
}

// Transform operations
Point3 Matrix4x4::transform_point(const Point3& p) const {
    // Use w=1 for points (affected by translation)
    float x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
    float y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
    float z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];
    float w = m[3][0] * p.x + m[3][1] * p.y + m[3][2] * p.z + m[3][3];

    // Perspective divide
    if (w != 1.0f && w != 0.0f) {
        return Point3(x / w, y / w, z / w);
    }
    return Point3(x, y, z);
}

Vector3 Matrix4x4::transform_vector(const Vector3& v) const {
    // Use w=0 for vectors (NOT affected by translation)
    float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
    float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
    float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;

    return Vector3(x, y, z);
}

Ray Matrix4x4::transform_ray(const Ray& r) const {
    Point3 new_origin = transform_point(r.origin);
    Vector3 new_direction = transform_vector(r.direction);
    // Ray constructor will normalize direction
    return Ray(new_origin, new_direction, r.entering, r.eta, r.o_primitive);
}
