#include "../include/math/transform.h"
#include "../include/math/floatutil.h"
#include <cmath>

// Constructors
Transform::Transform()
    : translation(0, 0, 0), rotation(0, 0, 0), scale_factors(1, 1, 1), is_identity_flag(true) {
    object_to_world = Matrix4x4::identity();
    world_to_object = Matrix4x4::identity();
}

Transform::Transform(const Vector3& translation, const Vector3& rotation, const Vector3& scale)
    : translation(translation), rotation(rotation), scale_factors(scale) {
    rebuild_matrices();
}

Transform::Transform(const json& transform_json) {
    // Parse translation (default: no translation)
    if (transform_json.contains("translate")) {
        translation = Vector3(transform_json["translate"]);
    } else {
        translation = Vector3(0, 0, 0);
    }

    // Parse rotation in degrees (default: no rotation)
    if (transform_json.contains("rotate")) {
        rotation = Vector3(transform_json["rotate"]);
    } else {
        rotation = Vector3(0, 0, 0);
    }

    // Parse scale (default: uniform scale of 1)
    if (transform_json.contains("scale")) {
        json scale_json = transform_json["scale"];
        if (scale_json.is_number()) {
            // Uniform scale
            float s = scale_json;
            scale_factors = Vector3(s, s, s);
        } else {
            // Non-uniform scale
            scale_factors = Vector3(scale_json);
        }
    } else {
        scale_factors = Vector3(1, 1, 1);
    }

    rebuild_matrices();
}

// Rebuild both transformation matrices analytically
void Transform::rebuild_matrices() {
    // Build object-to-world: T * R * S
    Matrix4x4 T = Matrix4x4::translation(translation.x, translation.y, translation.z);
    Matrix4x4 Rx = Matrix4x4::rotation_x(rotation.x);
    Matrix4x4 Ry = Matrix4x4::rotation_y(rotation.y);
    Matrix4x4 Rz = Matrix4x4::rotation_z(rotation.z);
    Matrix4x4 R = Rz * Ry * Rx;  // Z-Y-X order (yaw-pitch-roll)
    Matrix4x4 S = Matrix4x4::scale(scale_factors.x, scale_factors.y, scale_factors.z);

    object_to_world = T * R * S;

    // Build world-to-object: S^-1 * R^-1 * T^-1 (reverse order, analytical inverses)
    Matrix4x4 S_inv = Matrix4x4::scale(1.0f / scale_factors.x, 1.0f / scale_factors.y, 1.0f / scale_factors.z);
    Matrix4x4 R_inv = R.transpose();  // Rotation inverse is transpose (orthogonal matrix)
    Matrix4x4 T_inv = Matrix4x4::translation(-translation.x, -translation.y, -translation.z);

    world_to_object = S_inv * R_inv * T_inv;

    // Check if this is an identity transform
    is_identity_flag = check_if_identity();
}

bool Transform::check_if_identity() const {
    // Check if translation is zero
    if (!is_near_zero(translation.x) || !is_near_zero(translation.y) || !is_near_zero(translation.z)) {
        return false;
    }

    // Check if rotation is zero
    if (!is_near_zero(rotation.x) || !is_near_zero(rotation.y) || !is_near_zero(rotation.z)) {
        return false;
    }

    // Check if scale is (1, 1, 1)
    if (!equalf(scale_factors.x, 1.0f) || !equalf(scale_factors.y, 1.0f) || !equalf(scale_factors.z, 1.0f)) {
        return false;
    }

    return true;
}

// Apply transformations
Ray Transform::to_object_space(const Ray& world_ray) const {
    if (is_identity_flag) {
        return world_ray;
    }
    return world_to_object.transform_ray(world_ray);
}

Point3 Transform::to_world_space(const Point3& object_point) const {
    if (is_identity_flag) {
        return object_point;
    }
    return object_to_world.transform_point(object_point);
}

Vector3 Transform::to_world_space_normal(const Vector3& object_normal) const {
    if (is_identity_flag) {
        return object_normal;
    }
    // CRITICAL: Normals must be transformed by inverse transpose!
    // world_to_object is already the inverse, so we transpose it
    Matrix4x4 inverse_transpose = world_to_object.transpose();
    return inverse_transpose.transform_vector(object_normal);
}
