#pragma once
#include "math/vector3.h"
#include "light.h"

class LightD : public ILight {
    public:
        LightD();
        LightD(Vector3 l, float i);

        LightD(Vector3 l, float i, float c0, float c1, float c2);

        ~LightD();

        float atten(float d) const override;
        Ray get_shadow_ray(const Point3& x_p) const override;
        void print(std::ostream& os) const override;
        bool is_point() const override;
        float dist(const Point3& p) const override { return INFINITY; }
        float intensity() const override {return i;}

    private:
        Vector3 l;

        float i;

        float c0;
        float c1;
        float c2;
};