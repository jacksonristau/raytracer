#include "../include/math/transform.h"
#include "../include/math/floatutil.h"
#include <cmath>

// Constructors
Transform::Transform() : is_identity_flag(true) {
    object_to_world = Matrix4x4::identity();
    world_to_object = Matrix4x4::identity();
}

Transform::Transform(const Vector3& translation, const Vector3& rotation, const Vector3& scale) {
    build_matrices(translation, rotation, scale);
}

Transform::Transform(const json& transform_json) {
    Vector3 translation;
    Vector3 rotation;
    Vector3 scale;

    if (transform_json.contains("translate")) {
        translation = Vector3(transform_json["translate"]);
    } else {
        translation = Vector3(0, 0, 0);
    }

    if (transform_json.contains("rotate")) {
        rotation = Vector3(transform_json["rotate"]);
    } else {
        rotation = Vector3(0, 0, 0);
    }

    if (transform_json.contains("scale")) {
        json scale_json = transform_json["scale"];
        if (scale_json.is_number()) {
            float s = scale_json;
            scale = Vector3(s, s, s);
        } else {
            // non-uniform
            scale = Vector3(scale_json);
        }
    } else {
        scale = Vector3(1, 1, 1);
    }

    build_matrices(translation, rotation, scale);
}

void Transform::build_matrices(const Vector3& translation, const Vector3& rotation, const Vector3& scale) {
    Matrix4x4 T = Matrix4x4::translation(translation.x, translation.y, translation.z);
    Matrix4x4 Rx = Matrix4x4::rotation_x(rotation.x);
    Matrix4x4 Ry = Matrix4x4::rotation_y(rotation.y);
    Matrix4x4 Rz = Matrix4x4::rotation_z(rotation.z);
    Matrix4x4 R = Rz * Ry * Rx;
    Matrix4x4 S = Matrix4x4::scale(scale.x, scale.y, scale.z);

    object_to_world = T * R * S;

    Matrix4x4 S_inv = Matrix4x4::scale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
    Matrix4x4 R_inv = R.transpose();  // Rotation inverse is transpose (orthogonal matrix)
    Matrix4x4 T_inv = Matrix4x4::translation(-translation.x, -translation.y, -translation.z);

    world_to_object = S_inv * R_inv * T_inv;

    // Check if this is an identity transform
    is_identity_flag = check_if_identity(translation, rotation, scale);
}

bool Transform::check_if_identity(const Vector3& translation, const Vector3& rotation, const Vector3& scale) const {
    if (!is_near_zero(translation.x) || !is_near_zero(translation.y) || !is_near_zero(translation.z)) {
        return false;
    }

    if (!is_near_zero(rotation.x) || !is_near_zero(rotation.y) || !is_near_zero(rotation.z)) {
        return false;
    }

    if (!equalf(scale.x, 1.0f) || !equalf(scale.y, 1.0f) || !equalf(scale.z, 1.0f)) {
        return false;
    }

    return true;
}

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

    Matrix4x4 inverse_transpose = world_to_object.transpose();
    return inverse_transpose.transform_vector(object_normal);
}
