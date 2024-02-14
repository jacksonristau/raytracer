#include "point.h"
#include <math.h>

static const Point inf = Point(INFINITY, INFINITY, INFINITY);
const Point& Point::Inf() { return inf; }

Point::Point() {
    p[0] = 0.0;
    p[1] = 0.0;
    p[2] = 0.0;
}

Point::Point(float x, float y, float z) {
    p[0] = x;
    p[1] = y;
    p[2] = z;
}

Point::Point(const Point& p2) {
    p[0] = p2.x();
    p[1] = p2.y();
    p[2] = p2.z();
}

Point::~Point() {
}

bool Point::operator==(const Point& p2) const {
    return fabs(p[0] - p2.x()) < 0.001 && 
            fabs(p[1] - p2.y()) < 0.001 &&
            fabs(p[2] - p2.z()) < 0.001;
}

Point Point::operator-() {
    return Point(-x(), -y(), -z());
}

Point Point::operator=(const Point& p1) {
    p[0] = p1.x();
    p[1] = p1.y();
    p[2] = p1.z();
    return *this;
}

// p1.distance(p2)
float Point::distance(const Point& p1) {
    return sqrt(pow(p1.x() - x(), 2) + pow(p1.y() - y(), 2) + pow(p1.z() - z(), 2));
}

std::ostream & operator<< ( std::ostream& os, const Point& p) {
  return os << "(" << p.x() << ", " << p.y() << ", " << p.z() << ")";
}