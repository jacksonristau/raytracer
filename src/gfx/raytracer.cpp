#include "../include/gfx/color.h"
#include "../include/math/ray.h"
#include "../include/gfx/scene.h"
#include "../include/geometry/hit.h"

Color trace_ray(Ray r) {

	for (Primitive& prim : Scene::primitives) {
		Hit hit = prim.intersect(r);
		if (!hit.valid())
			return Scene::bkgcolor;

		Color reflection = Color(0, 0, 0);
		Color transmission = Color(0, 0, 0);

		if (prim.material->is_glossy()) {
			Ray reflected_ray (hit.x_pos, r.reflect(hit.normal));
			reflection = trace_ray(reflected_ray);
		}
		if (prim.material->is_transparent()) {
			Ray refracted_ray (hit.x_pos, )
		}
	}
}