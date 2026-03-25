#pragma once

#include "color.h"
#include "lights.h"
#include "../math/ray.h"
#include "../geometry/bvh.h"
#include "../geometry/primitive.h"

namespace Raytracer {
	extern bool wireframe_mode;
	extern BVH bvh;
	Color trace_ray(Ray r, int depth);
	Hit intersect_scene_naive(const Ray& r);
	float trace_shadow_ray_naive(const Ray& r, const ILight* l, float d);
}