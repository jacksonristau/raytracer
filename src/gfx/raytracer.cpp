#include "../include/gfx/color.h"
#include "../include/gfx/scene.h"
#include "../include/geometry/hit.h"
#include "../include/math/floatutil.h"
#include "../include/gfx/raytracer.h"

namespace Raytracer {
	bool wireframe_mode = false;
	int max_depth = 4;
	float edge_threshold = 0.02f;
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
		if (wireframe_mode){
			return nearest.is_edge ? nearest : Hit();
		}
		else {
			return nearest;
		}
	}

	float trace_shadow_ray_bvh(const Ray& r, const ILight* l, float d) {
		return bvh.traverse_shadow(bvh.nodes[0], r, l, d);
	}

	float trace_shadow_ray_naive(const Ray& r, const ILight* l, float d) {
		float s = 1.0f;
		Hit nearest = intersect_scene_bvh(r);
		if (!nearest.valid() || !is_negative(nearest.t))

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

	Color trace_debug(Ray r, int depth) {
		Hit hit = intersect_scene_bvh(r);
		if (!hit.valid())
			return Color(0.0f, 0.0f, 0.0f);
		
		if (wireframe_mode)
			return Color(0, 1, 0);

		Vector3 i = -r.direction;
		Vector3 shading_normal = hit.normal;
		float ndoti = shading_normal.dot(i);

		// Flip normal if we hit the back face (ray is inside the mesh)
		if (ndoti < 0) {
			shading_normal = -shading_normal;
			ndoti = -ndoti;
		}

		auto material = hit.primitive->material;
		// umn texture
		if (r.entering == false && hit.primitive->material->is_uniform() == false) {
			return Color(0, 1, 0);
		}
		//Color reflection = Color(0, 0, 0);
		Color transmission = Color(0, 0, 0);
		Point3 reflect_origin = hit.x_pos + 1e-3 * shading_normal;
		Point3 refract_origin = hit.x_pos - 1e-3 * shading_normal;
		// if (material->is_glossy() && depth < max_depth) {
		// 	float fr = fresnel(ndoti, material->eta(), 0.0, true);
		// 	Ray reflected_ray(reflect_origin, r.reflect(shading_normal), r.entering, r.eta, hit.primitive);
		// 	reflection = fr * trace_ray(reflected_ray, depth + 1);
		// }
		if (material->is_transparent() && depth < max_depth) {
			float nt = r.entering ? material->eta() : Scene::bkgeta;
			float fr = fresnel(ndoti, r.eta, nt, false);
			Vector3 refract_dir = r.refract(shading_normal, ndoti, r.eta, nt);
			if (refract_dir.is_zero()) {
				Ray tir_ray(reflect_origin, r.reflect(shading_normal), r.entering, r.eta, hit.primitive);
				transmission = fr * trace_debug(tir_ray, depth + 1) + Color(0.0f, 0.2f, 0.0f);
			}
			else {
				Ray refracted_ray(refract_origin, refract_dir, !r.entering, nt, hit.primitive);
				transmission = (1 - fr) * (1 - material->alpha()) * trace_debug(refracted_ray, depth + 1);
			}
		}

		return material->evaluate_debug(hit, Color(0,0,0), transmission);
	}

	Color trace_ray(Ray r, int depth) {
		Hit hit = intersect_scene_bvh(r);
		if (!hit.valid())
			return Scene::bkgcolor;
		
		if (wireframe_mode)
			return Color(0, 1, 0);

		Vector3 i = -r.direction;
		Vector3 shading_normal = hit.normal;
		float ndoti = shading_normal.dot(i);

		// Flip normal if we hit the back face (ray is inside the mesh)
		if (ndoti < 0) {
			shading_normal = -shading_normal;
			ndoti = -ndoti;
		}

		auto material = hit.primitive->material;

		Color reflection = Color(0, 0, 0);
		Color transmission = Color(0, 0, 0);
		Point3 reflect_origin = hit.x_pos + Eps * shading_normal;
		Point3 refract_origin = hit.x_pos - Eps * shading_normal;
		if (material->is_glossy() && depth < max_depth) {
			float fr = fresnel(ndoti, material->eta(), 0.0, true);
			Ray reflected_ray(reflect_origin, r.reflect(shading_normal), r.entering, r.eta, hit.primitive);
			reflection = fr * trace_ray(reflected_ray, depth + 1);
		}
		if (material->is_transparent() && depth < max_depth) {
			float nt = r.entering ? material->eta() : Scene::bkgeta;
			float fr = fresnel(ndoti, r.eta, nt, false);
			Vector3 refract_dir = r.refract(shading_normal, ndoti, r.eta, nt);
			if (refract_dir.is_zero()) {
				Ray tir_ray(reflect_origin, r.reflect(shading_normal), r.entering, r.eta, hit.primitive);
				transmission = fr * trace_ray(tir_ray, depth + 1);
			}
			else {
				Ray refracted_ray(refract_origin, refract_dir, !r.entering, nt, hit.primitive);
				transmission = (1 - fr) * (1 - material->alpha()) * trace_ray(refracted_ray, depth + 1);
			}
		}
		
		return material->evaluate(hit, reflection, transmission);
	}
}