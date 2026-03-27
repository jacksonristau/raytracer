#include "../include/geometry/primitive.h"
#include "../include/geometry/hit.h"
#include "../include/geometry/shape.h"
#include "../include/math/floatutil.h"
#include "../include/gfx/material.h"

Hit Primitive::intersect(const Ray& world_ray) const {
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
    h.t = (h.x_pos - world_ray.origin).dot(world_ray.direction);
    h.primitive = this;

    return h;
}

AABB Primitive::get_aabb() const {
    AABB local = geometry->get_aabb();
    if (transform.is_identity()) return local;

    Point3 corners[8] = {
          {local.min.x, local.min.y, local.min.z},
          {local.max.x, local.min.y, local.min.z},
          {local.min.x, local.max.y, local.min.z},
          {local.min.x, local.min.y, local.max.z},
          {local.max.x, local.max.y, local.min.z},
          {local.max.x, local.min.y, local.max.z},
          {local.min.x, local.max.y, local.max.z},
          {local.max.x, local.max.y, local.max.z},
      };

      Point3 min = Point3::infinity();
      Point3 max = Point3::negative_infinity();
      for (auto& c : corners) {
        Point3 wc = transform.to_world_space(c);
        min = Point3::min(min, wc);
        max = Point3::max(max, wc);
      }
      return AABB(min, max);
}