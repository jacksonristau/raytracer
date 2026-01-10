#pragma once
#include "shape.h"
#include "aabb.h"

class Ray;
class Hit;
class Point2;

class Sphere : public IShape {
    public:
        Sphere();
        Sphere(const Point3& center, float radius);
        Sphere(json sphere_json);
        virtual ~Sphere();

        // equality
        bool operator==(const Sphere& s1) const;

        Point2 get_uv(const Point3& point) const;

        int get_type() const override {return SHAPE_TYPE_SPHERE;}
        Hit intersect(const Ray& r, bool with_uv = false) const override;
        Vector3 normal(const Hit& h, const Point3& p) const;
        AABB get_aabb() const override;
        Point3 get_centroid() const override { return center; }

        Point3 center;
        float radius;
};