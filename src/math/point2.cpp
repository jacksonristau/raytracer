#include "point2.h"
#include "floatutil.h"
#include <cmath>

Point2::Point2() : x(0.0f), y(0.0f) {}

Point2::Point2(float x, float y) : x(x), y(y) {}

Point2::Point2(const Point2& p1) {
    x = p1.x;
    y = p1.y;
}

Point2::~Point2() {}

Point2 Point2::operator=(const Point2& p1) {
    x = p1.x;
    y = p1.y;
    return *this;
}

bool operator== (const Point2& p1, const Point2& p2){
	return equalf(p1.x, p2.x) &&
		   equalf(p1.y, p2.y);
}

Point2 Point2::operator- () const {
	return { -x, -y };
}

float distance(const Point2& p1, const Point2& p2) {
	return sqrt(
		(p1.x - p2.x) * (p1.x - p2.x) +
		(p1.y - p2.y) * (p1.y - p2.y)
	);
}

Point2 operator*(float s, const Point2& p1) {
	return Point2(s * p1.x, s * p1.y);
}

Point2 operator*(const Point2& p1, float s) {
	return Point2(p1.x * s, p1.y * s);
}

std::ostream& operator<<(std::ostream& os, const Point2& p) {
	return os << "(" << p.x << ", " << p.y << ")";
}