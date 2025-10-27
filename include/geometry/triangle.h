#pragma once

#include "hit.h"
#include "shape.h"
#include "mesh.h"
#include "../math/vector3.h"
#include "../math/ray.h"

class Triangle : IShape {
	public:
		Triangle(const Mesh* mesh, int index) : mesh(mesh), index(index){}

		Hit intersect(const Ray& r) const override;

		Vector3 normal(const Hit& h, const Point3& p) const override;

		const Mesh* mesh;
		int index;
};