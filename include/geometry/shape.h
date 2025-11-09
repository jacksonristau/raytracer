#pragma once
#include "hit.h"
#include "../math/ray.h"
#include "../math/point3.h"
#include "../math/vector3.h"

// abstract base class for geometry
class IShape {
	public:
		virtual int get_type() const = 0;
		virtual Hit intersect(const Ray& r, bool with_uv = false) const = 0;
		virtual Vector3 normal(const Hit& h, const Point3& p) const = 0;
};