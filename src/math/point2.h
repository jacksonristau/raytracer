#pragma once

#include <iostream>

class Point2 {
    public:
        Point2();
        Point2(float x, float y);
        Point2(const Point2& p1);

        virtual ~Point2();

        // negation
        Point2 operator- () const;

        // assignment
        Point2 operator= (const Point2& p1);

        float x, y;
};

float distance(const Point2& p1, const Point2& p2);

bool operator== (const Point2& p1, const Point2& p2);

Point2 operator* (float s, const Point2& p1);

Point2 operator* (const Point2& p1, float s);

std::ostream& operator<< (std::ostream& os, const Point2& p);