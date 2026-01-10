#pragma once
#include "hit.h"
#include "../math/ray.h"
#include "../math/point3.h"
#include "../math/vector3.h"
#include "aabb.h"

// abstract base class for geometry
class IShape {
	public:
		virtual int get_type() const = 0;
		virtual Hit intersect(const Ray& r, bool with_uv = false) const = 0;
		virtual AABB get_aabb() const = 0;
		virtual Point3 get_centroid() const = 0;
};

// Shape type constants
constexpr int SHAPE_TYPE_SPHERE = 1;
constexpr int SHAPE_TYPE_PLANE  = 2;
constexpr int SHAPE_TYPE_TRIANGLE = 3;
constexpr int SHAPE_TYPE_BOX = 4;
constexpr int SHAPE_TYPE_CYLINDER = 5;
constexpr int SHAPE_TYPE_CONE = 6;