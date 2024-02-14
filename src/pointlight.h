#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"
#include "point.h"

class PointLight : public Light {
    public:
        PointLight(float intensity, const Point& position)
            : Light(intensity), p(position) {}

        virtual float x() const {return p.x();}
        virtual float y() const {return p.y();}
        virtual float z() const {return p.y();}
        virtual float w() const {return 1;}

    private:
        Point p;
};

#endif 