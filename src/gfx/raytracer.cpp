#include "../include/gfx/color.h"
#include "../include/math/ray.h"
#include "../include/gfx/scene.h"
#include "../include/geometry/hit.h"
#include "../include/math/floatutil.h"
#include "../include/gfx/raytracer.h"

namespace Raytracer {
	Color trace_ray(Ray r, int depth) {
		Hit hit = r.intersect_scene_naive();
		if (!hit.valid())
			return Scene::bkgcolor;

		Vector3 v(r.direction);
		v = -v;
		float ndotv = hit.normal.dot(v);

		Material* material = hit.primitive->material;

		Color reflection = Color(0, 0, 0);
		Color transmission = Color(0, 0, 0);
		if (material->is_glossy() && depth < 6) {
			float fr = fresnel(ndotv, material->eta(), 0.0, true);
			Ray reflected_ray(hit.x_pos, r.reflect(hit.normal), r.entering, r.eta, hit.primitive);
			reflection = fr * trace_ray(reflected_ray, depth + 1);
		}
		if (material->is_transparent() && depth < 6) {
			float nt = r.entering ? material->eta() : Scene::bkgeta;
			float fr = fresnel(ndotv, r.eta, nt, false);
			Ray refracted_ray(hit.x_pos, r.refract(hit.normal, ndotv, r.eta, nt), !r.entering, nt, hit.primitive);
			transmission = (1 - fr) * (1 - material->alpha()) * trace_ray(refracted_ray, depth + 1);
		}

		return material->evaluate(hit, reflection, transmission);
	}

	Hit intersect_scene_naive(const Ray& r) {
		Hit nearest = Hit();
		for (Primitive& prim : Scene::primitives) {
			if (prim == *r.o_primitive)
				continue;
			Hit hit = prim.intersect(r);
			if (!hit.valid())
				continue;

			if (!nearest.valid()) {
				nearest = hit;
				continue;
			}

			if (hit.t < nearest.t)
				nearest = hit;
		}
		return nearest;
	}

	float trace_shadow_ray_naive(const Ray& r, const ILight* l, float d) {
		float s = 1.0f;
		for (Primitive& prim : Scene::primitives) {
			if (prim == *r.o_primitive)
				continue;
			Hit hit = prim.intersect(r);
			if (!hit.valid() || !is_greater_than_zero(hit.t))
				continue;

			if (l->is_point() && hit.t > d)
				continue;

			s *= (1 - prim.material->alpha());
		}
	}

	float fresnel(float cosi, float ni, float nt, bool reflect) {
		float fo;
		if (reflect) {
			fo = ((ni - 1) / (ni + 1));
		}
		else {
			fo = (nt - ni) / (nt + ni);
		}
		fo = fo * fo;
		return fo + (1 - fo) * std::powf((1 - cosi), 5);
	}
}