#pragma once

#include "matrix4x4.h"
#include "vector3.h"
#include "point3.h"
#include "ray.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Transform {
public:
    Transform();  // Identity transform
    Transform(const Vector3& translation, const Vector3& rotation, const Vector3& scale);
    Transform(const json& transform_json);  // Parse from JSON

    // Apply transformations
    Ray to_object_space(const Ray& world_ray) const;
    Point3 to_world_space(const Point3& object_point) const;
    Vector3 to_world_space_normal(const Vector3& object_normal) const;

    // Query
    bool is_identity() const { return is_identity_flag; }

    const Matrix4x4& get_object_to_world() const { return object_to_world; }
    const Matrix4x4& get_world_to_object() const { return world_to_object; }

private:
    Vector3 translation;
    Vector3 rotation;      // Euler angles in degrees
    Vector3 scale_factors;

    Matrix4x4 object_to_world;  // T * R * S
    Matrix4x4 world_to_object;  // S^-1 * R^-1 * T^-1

    bool is_identity_flag;

    void rebuild_matrices();  // Builds both matrices analytically
    bool check_if_identity() const;
};
