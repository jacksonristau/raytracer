#include "../include/gfx/color.h"
#include "../include/gfx/scene.h"
#include "../include/geometry/hit.h"
#include "../include/math/floatutil.h"
#include "../include/gfx/raytracer.h"

namespace Raytracer {
	bool wireframe_mode = false;
	BVH bvh = BVH();

	Hit intersect_scene_naive(const Ray& r) {
		Hit nearest = Hit();
		for (Primitive& prim : Scene::primitives) {
			if (r.o_primitive != nullptr && prim == *r.o_primitive)
				continue;
			Hit hit = prim.intersect(r);
			if (!hit.valid())
				continue;
			if (wireframe_mode && !hit.is_edge)
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

	Hit intersect_scene_bvh(const Ray& r) {
		Hit nearest = Hit::infinity();
		bvh.traverse(bvh.nodes[0], r, nearest);
		return nearest;
	}

	float trace_shadow_ray_naive(const Ray& r, const ILight* l, float d) {
		float s = 1.0f;
		for (Primitive& prim : Scene::primitives) {
			if (r.o_primitive != nullptr && prim == *r.o_primitive)
				continue;
			Hit hit = prim.intersect(r);
			if (!hit.valid() || !is_greater_than_zero(hit.t))
				continue;

			if (l->is_point() && hit.t > d)
				continue;

			s *= (1 - prim.material->alpha());
		}
		return s;
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

	Color trace_ray(Ray r, int depth) {
		Hit hit = intersect_scene_bvh(r);
		if (!hit.valid())
			return Scene::bkgcolor;
		
		if (wireframe_mode)
			return Color(0, 1, 0);

		Vector3 i = -r.direction;
		float ndoti = hit.normal.dot(i);

		auto material = hit.primitive->material;

		Color reflection = Color(0, 0, 0);
		Color transmission = Color(0, 0, 0);
		if (material->is_glossy() && depth < 6) {
			float fr = fresnel(ndoti, material->eta(), 0.0, true);
			Ray reflected_ray(hit.x_pos, r.reflect(hit.normal), r.entering, r.eta, hit.primitive);
			reflection = fr * trace_ray(reflected_ray, depth + 1);
		}
		if (material->is_transparent() && depth < 6) {
			float nt = r.entering ? material->eta() : Scene::bkgeta;
			float fr = fresnel(ndoti, r.eta, nt, false);
			Ray refracted_ray(hit.x_pos, r.refract(hit.normal, ndoti, r.eta, nt), !r.entering, nt, hit.primitive);
			transmission = (1 - fr) * (1 - material->alpha()) * trace_ray(refracted_ray, depth + 1);
		}
		
		return material->evaluate(hit, reflection, transmission);
	}
}