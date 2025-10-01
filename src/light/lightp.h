#pragma once
#include "math/point3.h"
#include "light.h"

class LightP : public ILight {
    public:
        LightP();
        LightP(Point3 l, float i);
        LightP(Point3 l, float i, float c0, float c1, float c2);

        ~LightP();

        float atten(float d) const override;
        Ray get_shadow_ray(const Point3& x_p) const override;
        void print(std::ostream& os) const override;
        bool is_point() const override;
        float dist(const Point3&) const override;
        float intensity() const override {return i;}

    private:
        Point3 l;

        float i;

        float c0;
        float c1;
        float c2;
};