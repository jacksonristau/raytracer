#pragma once

#include "hit.h"
#include "shape.h"
#include "mesh.h"
#include "../gfx/tiny_obj_loader.h"
#include "../math/vector3.h"
#include "../math/ray.h"

// keeping triangle somewhat small as there will be a lot
// 8 byte ptr + 4 byte int + 8 byte virtual function ptr = 20 byte triangle
// kind of massive but we'll see
class Triangle : public IShape {
	public:
		Triangle(const Mesh* mesh, int index) : mesh(mesh), index(index){}

		Point3 get_vertex(int v) const;
		Vector3 get_normal(int n) const;
		Point2 get_uv(int t) const;

		bool has_normal() const;
		bool has_uv() const;

		int get_type() const override {return 1;}
		Hit intersect(const Ray& r, bool with_uv = false) const override;
		Point2 compute_planar_uv(const Point3& p, const Vector3& normal) const;

		const Mesh* mesh;
		int index;
};
