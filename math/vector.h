#ifndef VECTOR_H
#define VECTOR_H

#include "point.h"
#include <iostream>

class Vector {
    public:
        Vector();

        Vector(float x, float y, float z);

        Vector(const Vector& v2);

        virtual ~Vector();

        // equality
        bool operator==(const Vector& v) const;

        // negation
        Vector operator-();

        // addition
        Vector operator+(const Vector& v1);

        Vector operator=(const Vector& v1);

        void normalize();

        Vector cross(const Vector& v1);

        float dot(const Vector& v1);

        float x() const {return v[0];}
        float y() const {return v[1];}
        float z() const {return v[2];}

    private:
        float v[3];
};

Point operator+(const Point& p1, const Vector& v1);

Point operator-(const Point& p1, const Vector& v1);

Vector operator-(const Point& p1, const Point& p2);

Vector operator*(float s, const Vector& v1);

std::ostream & operator<< ( std::ostream &os, const Vector &v);

#endif