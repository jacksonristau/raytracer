#pragma once
#include "material.h"
#include "math/ray.h"
// the base for objects. 
class Intersection;
class IShape;

class Primitive {
	public:
		Primitive();
		~Primitive();

		Intersection intersect(const Ray& r);

		IShape& geometry;
		int mtl_id;
};