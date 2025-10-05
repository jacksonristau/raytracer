#include "point3.h"
#include "vector3.h"
#include "floatutil.h"

Point3::Point3() : x(0.0f), y(0.0f), z(0.0f) {}

Point3::Point3(float x, float y, float z) : x(x), y(y), z(z) {}

Point3::Point3(const Point3& p1) {
	x = p1.x;
	y = p1.y;
	z = p1.z;
}

Point3::~Point3(){}

Point3 Point3::operator=(const Point3& p1) {
	x = p1.x;
	y = p1.y;
	z = p1.z;
	return *this;
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
	return Point3(p1.x * s, p1.y * s, p1.z * s);
}

Point3 operator*(const Point3& p1, float s) {
	return Point3(p1.x * s, p1.y * s, p1.z * s);
}

std::ostream& operator<<(std::ostream& os, const Point3& p) {
	return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
}