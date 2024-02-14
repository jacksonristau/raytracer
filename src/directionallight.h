#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "light.h"
#include "vector.h"

class DirectionalLight : public Light {
    public:
        DirectionalLight(float intensity, const Vector& direction)
            : Light(intensity), d(direction) {}

        virtual float x() const {return d.x();}
        virtual float y() const {return d.y();}
        virtual float z() const {return d.y();}
        virtual float w() const {return 0;}

    private:
        Vector d;
};

#endif 