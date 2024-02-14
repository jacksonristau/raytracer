#ifndef POINT_H
#define POINT_H

#include <ostream>

class Point {
    public:
        Point();

        Point(float x, float y, float z);

        Point(const Point& p2);

        Point(float *ptr);

        virtual ~Point();


        // equality
        bool operator==(const Point& p) const;

        // negation
        Point operator-();

        Point operator=(const Point& p1);

        float distance(const Point& p1);

        float x() const {return p[0];}
        float y() const {return p[1];}
        float z() const {return p[2];}
        float w() const {return 1;}

        // shortcut
        static const Point& Inf();

    private:
        float p[3];
};

std::ostream & operator<< ( std::ostream &os, const Point &p);

#endif