#pragma once
#include "../gfx/material.h"
#include "../math/ray.h"
#include "hit.h"
// the base for objects. 
class Intersection;
class IShape;

class Primitive {
	public:
		Primitive();
		~Primitive();

		Hit intersect(const Ray& r);


		IShape* geometry;
		Material* material;
		int id;

		bool operator==(const Primitive& other) const {
			return id == other.id;
		}
};