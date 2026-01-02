#pragma once

#include "color.h"
#include "lights.h"
#include "../math/ray.h"

namespace Raytracer {
	extern bool wireframe_mode;
	Color trace_ray(Ray r, int depth);
	Hit intersect_scene_naive(const Ray& r);
	float trace_shadow_ray_naive(const Ray& r, const ILight* l, float d);
}