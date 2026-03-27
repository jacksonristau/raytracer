#include "../include/math/point3.h"
#include "../include/math/vector3.h"
#include "../include/math/floatutil.h"
#include <limits>

Point3::Point3() : x(0.0f), y(0.0f), z(0.0f) {}

Point3::Point3(float x, float y, float z) : x(x), y(y), z(z) {}

Point3::Point3(json p) {
	x = p[0];
	y = p[1];
	z = p[2];
}

Vector3 operator- (const Point3& p1, const Point3& p2) {
	return Vector3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

Point3 operator- (const Point3& p1, const Vector3& v1) {
	return Point3(p1.x - v1.x, p1.y - v1.y, p1.z - v1.z);
}

bool operator== (const Point3& p1, const Point3& p2){
	return equalf(p1.x, p2.x) &&
		   equalf(p1.y, p2.y) &&
		   equalf(p1.z, p2.z);
}

Point3 Point3::operator- () const {
	return { -x, -y, -z };
}

float distance(const Point3& p1, const Point3& p2) {
	return sqrt(
		(p1.x - p2.x) * (p1.x - p2.x) +
		(p1.y - p2.y) * (p1.y - p2.y) +
		(p1.z - p2.z) * (p1.z - p2.z)
	);
}

Point3 operator+ (const Point3& p1, const Vector3& v1) {
	return { p1.x + v1.x, p1.y + v1.y, p1.z + v1.z };
}

Point3 operator*(float s, const Point3& p1) {
	return Point3(s * p1.x, s * p1.y, s * p1.z);
}

Point3 operator*(const Point3& p1, float s) {
	return Point3(p1.x * s, p1.y * s, p1.z * s);
}

std::ostream& operator<<(std::ostream& os, const Point3& p) {
	return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
}

Point3 Point3::max(const Point3& p1, const Point3& p2){
	Point3 res = Point3();
	res.x = fmax(p1.x, p2.x);
	res.y = fmax(p1.y, p2.y);
	res.z = fmax(p1.z, p2.z);

	return res;
}

Point3 Point3::min(const Point3& p1, const Point3& p2){
	Point3 res = Point3();
	res.x = fmin(p1.x, p2.x);
	res.z = fmin(p1.z, p2.z);
	res.y = fmin(p1.y, p2.y);

	return res;
}

Point3 Point3::infinity() {
	float inf = std::numeric_limits<float>::infinity();
	return Point3(inf, inf, inf);
}

Point3 Point3::negative_infinity() {
	float inf = std::numeric_limits<float>::infinity();
	return Point3(-inf, -inf, -inf);
}