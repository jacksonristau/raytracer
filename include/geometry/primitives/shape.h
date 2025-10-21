#pragma once
#include "intersection.h"
#include "math/ray.h"
#include "math/point3.h"
#include "math/vector3.h"

// abstract base class for geometry
class IShape {
	public:
		virtual float intersect(const Ray& r) const = 0;
		virtual Vector3 get_normal(const Point3& p) const = 0;
};