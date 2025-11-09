#pragma once
#include "shape.h"

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

        int get_type() const override {return 0;}
        Hit intersect(const Ray& r, bool with_uv = false) const override;
        Vector3 normal(const Hit& h, const Point3& p) const override;

        Point3 center;
        float radius;
};