#pragma once
#include "intersection.h"
#include "hit.h"
#include "../math/ray.h"
#include "../math/point3.h"
#include "../math/vector3.h"

// abstract base class for geometry
class IShape {
	public:
		virtual bool intersect(const Ray& r, const Hit& out) const = 0;
		virtual Vector3 normal(const Hit& h, const Point3& p) const = 0;
};