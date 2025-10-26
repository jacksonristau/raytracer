#pragma once

#include "../math/vector3.h"
#include "../math/ray.h"
#include "mesh.h"

class Triangle : IShape {
	public:
		Triangle(const Mesh* mesh, int i0, int i1, int i2) : mesh(mesh), i0(i0), i1(i1), i2(i2) {}

		bool intersect(const Ray& r, Hit& out) const;

		Vector3 normal(const Hit& h, const Point3& p) const;

		const Mesh* mesh;
		int i0, i1, i2;
};