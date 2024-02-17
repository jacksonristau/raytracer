#ifndef SPHERE_H
#define SPHERE_H

#include "vector.h"

class Sphere {
    public:
        Sphere();

        Sphere(const Vector& center, float radius);

        Sphere(const Vector& center, float radius, int m_index);

        virtual ~Sphere();

        // equality
        bool operator==(const Sphere& s2) const;

        void set(const Vector& center, float radius);

        Vector center() const {return c;}
        float radius() const {return r;}
        int material() const {return m;} 

    private:
        Vector c;
        float r;
        int m;
};
#endif