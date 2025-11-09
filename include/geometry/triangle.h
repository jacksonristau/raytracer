#pragma once

#include "hit.h"
#include "shape.h"
#include "mesh.h"
#include "../gfx/tiny_obj_loader.h"
#include "../math/vector3.h"
#include "../math/ray.h"

class Triangle : public IShape {
	public:
		Triangle(const Mesh* mesh, int index) : mesh(mesh), index(index){}

		Point3 get_vertex(int v) const;
		Vector3 get_normal(int n) const;
		Point2 Triangle::get_uv(int t) const;

		Hit intersect(const Ray& r) const override;

		Vector3 normal(const Hit& h, const Point3& p) const override;

		const Mesh* mesh;
		int index;
};