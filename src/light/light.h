#pragma once
#include "math/vector3.h"
#include "math/ray.h"

class ILight {
    public:
        virtual ~ILight() {};

        virtual float atten(float d) const = 0;
        virtual Ray get_shadow_ray(const Point3& x_p) const = 0;
    
        // If you want to use operator<< polymorphically, declare a pure virtual print method:
        virtual void print(std::ostream& os) const = 0;
        virtual bool is_point() const = 0;
        virtual float dist(const Point3& p) const = 0;
        virtual float intensity() const = 0;
};

// Non-member operator<< for ILight base class
inline std::ostream& operator<<(std::ostream& os, const ILight& light) {
    light.print(os);
    return os;
}