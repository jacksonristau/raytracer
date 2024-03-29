#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

class Light {
    public:
        Light();
        Light(Vector l, float intensity);
        virtual ~Light();

        float intensity() const { return i; }
        Vector l() const { return v; }
        inline float w() const { return v.w(); }

        void set_att(float c1, float c2, float c3);
        float atten(float d) const;

    private:
        Vector v;
        float att[3];
        float i;
};

std::ostream & operator<< ( std::ostream &os, const Light &l);

#endif 