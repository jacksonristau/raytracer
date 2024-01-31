#ifndef SPHERE_H
#define SPHERE_H

#include "point.h"

class Sphere {
    public:
        Sphere();

        Sphere(const Point& center, float radius);

        Sphere(const Point& center, float radius, int m_index);

        virtual ~Sphere();

        // equality
        bool operator==(const Sphere& s2) const;

        void set(const Point& center, float radius);

        Point center() const {return c;}
        float radius() const {return r;}
        int material() const {return m;} 

    private:
        Point c;
        float r;
        int m;
};
#endif