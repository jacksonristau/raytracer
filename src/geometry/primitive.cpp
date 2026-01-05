#include "../include/geometry/primitive.h"
#include "../include/geometry/hit.h"
#include "../include/geometry/shape.h"
#include "../include/math/floatutil.h"
#include "../include/gfx/material.h"

Hit Primitive::intersect(const Ray& world_ray) {
    bool with_uv = !material->is_uniform();

    // 1. Transform ray to object space
    Ray object_ray = transform.to_object_space(world_ray);

    // 2. Intersect in object space (existing code unchanged)
    Hit h = geometry->intersect(object_ray, with_uv);

    if (!h.valid()) {
        return h;  // No hit
    }

    // 3. Transform hit back to world space
    h.x_pos = transform.to_world_space(h.x_pos);
    h.normal = transform.to_world_space_normal(h.normal);
    h.normal.normalize();  // Re-normalize after transformation
    h.r = world_ray;  // Store original world-space ray
    h.primitive = this;

    return h;
}