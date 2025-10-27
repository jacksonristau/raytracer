#pragma once
#include "shape.h"
#include "intersection.h"
#include "../math/point3.h"
#include "../math/ray.h"

class Sphere : public IShape {
    public:
        Sphere();
        Sphere(const Point3& center, float radius);
        virtual ~Sphere();

        // equality
        bool operator==(const Sphere& s1) const;

        // returns texture coordinates at a point on the sphere using polar coordinates
        int get_uv(const Point3& point, float* uv) const;
        Hit intersect(const Ray& r) const override;
        Vector3 normal(const Hit& h, const Point3& p) const override;

        Point3 center;
        float radius;
};