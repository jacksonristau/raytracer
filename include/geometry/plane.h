#pragma once
#include "../include/math/point3.h"
#include "../include/math/vector3.h"
#include "shape.h"

class Plane : public IShape {
    public:
        Plane();
        Plane(Point3 p, Vector3 n);

        float intersect(const Ray& r) const override;

        Point3 pos;
        Vector3 normal;
};