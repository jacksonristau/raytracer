#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <cmath>

#include "../include/math/vector3.h"
#include "../include/math/point3.h"
#include "../include/math/floatutil.h"

constexpr float EPS = 1e-6f;

TEST_CASE("Point3: constructors, equality, negation and assignment") {
	Point3 p0; // default
	REQUIRE(p0.x == 0.0f);
	REQUIRE(p0.y == 0.0f);
	REQUIRE(p0.z == 0.0f);

	Point3 p1(1.5f, -2.0f, 3.25f);
	REQUIRE(equalf(p1.x, 1.5f));
	REQUIRE(equalf(p1.y, -2.0f));
	REQUIRE(equalf(p1.z, 3.25f));

	Point3 p2(p1); // copy ctor
	REQUIRE(p2 == p1);

	Point3 p3;
	p3 = p1; // assignment
	REQUIRE(p3 == p1);

	Point3 pneg = -p1;
	REQUIRE(equalf(pneg.x, -p1.x));
	REQUIRE(equalf(pneg.y, -p1.y));
	REQUIRE(equalf(pneg.z, -p1.z));
}

TEST_CASE("Point3 distance") {
	Point3 p(3.0f, 0.0f, 4.0f); // length 5
	
	REQUIRE(equalf(distance(Point3(0.0f, 0.0f, 0.0f), p), 5.0f));
}

TEST_CASE("Point3: arithmetic operators (+, -, scalar *)") {
	Point3  p1(1.0f, 1.0f, 1.0f);
    Point3  p2(2.0f, 0.0f, 1.0f);
    Vector3 v1(1.0f, 0.0f, 0.0f);

    REQUIRE(p1 * 2.0f == Point3(2.0f, 2.0f, 2.0f));

    REQUIRE(p1 - p2 == Vector3(-1.0f, 1.0f, 0.0f));

    REQUIRE(p1 + v1 == Point3(2.0f, 1.0f, 1.0f));
    REQUIRE(p1 - v1 == Point3(0.0f, 1.0f, 1.0f));
}

TEST_CASE("Point3: ostream prints components") {
	Point3 p(1.0f, -2.5f, 3.75f);
	std::ostringstream oss;
	oss << p;
	std::string out = oss.str();
	// basic sanity checks on formatting
	REQUIRE(out.front() == '(');
	REQUIRE(out.find(",") != std::string::npos);
	REQUIRE(out.back() == ')');
}
