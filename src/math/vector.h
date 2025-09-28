#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

class Vector {
    public:
        Vector();

        Vector(float x, float y, float z, float w = 0.0);

        Vector(const Vector& v2);

        virtual ~Vector();

        // equality
        bool operator==(const Vector& v) const;

        // negation
        Vector operator-();

        // addition
        Vector operator+(const Vector& v1);

        // assignment
        Vector operator=(const Vector& v1);

        void normalize();

        Vector cross(const Vector& v1) const;

        float distance(const Vector& v1) const;

        float dot(const Vector& v1) const;

        float x() const {return v[0];}
        float y() const {return v[1];}
        float z() const {return v[2];}
        inline float w() const {return v[3];}

        static const Vector& Inf();

    private:
        float v[4];
};

Vector operator+(const Vector& v1, const Vector& v2);

Vector operator-(const Vector& v1, const Vector& v2);

Vector operator*(float s, const Vector& v1);

std::ostream & operator<< ( std::ostream &os, const Vector &v);

#endif