#pragma once
#include "../math/point3.h"
#include "../geometry/mesh.h"

class BoundingBox {
    public:
        BoundingBox(Mesh m);
        Point3 center() const;
        Point3 min;
        Point3 max;
};