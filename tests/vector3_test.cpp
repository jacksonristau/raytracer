#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <cmath>

#include "../include/math/vector3.h"
#include "../include/math/point3.h"
#include "../include/math/floatutil.h"

TEST_CASE("Vector3: constructors, equality, negation and assignment") {
	Vector3 v0; // default
	REQUIRE(v0 == Vector3(0.0f, 0.0f, 0.0f));

	Vector3 v1(1.5f, -2.0f, 3.25f);
	REQUIRE(equalf(v1.x, 1.5f));
	REQUIRE(equalf(v1.y, -2.0f));
	REQUIRE(equalf(v1.z, 3.25f));

	Vector3 v2(v1); // copy ctor
	REQUIRE(v2 == v1);

	Vector3 v3;
	v3 = v1; // assignment
	REQUIRE(v3 == v1);

	Vector3 vneg = -v1;
	REQUIRE(equalf(vneg.x, -v1.x));
	REQUIRE(equalf(vneg.y, -v1.y));
	REQUIRE(equalf(vneg.z, -v1.z));
}

TEST_CASE("Vector3: normalize and distance") {
	Vector3 v(3.0f, 0.0f, 4.0f); // length 5
	v.normalize();
	// length should be 1.0
	REQUIRE(equalf(v.distance(Vector3(0.0f, 0.0f, 0.0f)), 1.0f));
	// normalized components should be 3/5 and 4/5 where appropriate
	REQUIRE(equalf(v.x, (3.0f / 5.0f)));
	REQUIRE(equalf(v.z, (4.0f / 5.0f)));
}

TEST_CASE("Vector3: cross and dot products") {
	Vector3 i(1.0f, 0.0f, 0.0f);
	Vector3 j(0.0f, 1.0f, 0.0f);
	Vector3 k(0.0f, 0.0f, 1.0f);

	Vector3 cross = i.cross(j);
	REQUIRE(cross == k);

	REQUIRE(equalf(i.dot(j), 0.0f));
	REQUIRE(equalf(i.dot(i), 1.0f));

	Point3 p(2.0f, 0.0f, -1.0f);
	Vector3 v(1.0f, 0.0f, 0.0f);
	// dot with Point3 delegates to the point coordinates
	REQUIRE(equalf(v.dot(p), 2.0f));
}

TEST_CASE("Vector3: arithmetic operators (+, -, scalar *)") {
	Vector3 a(1.0f, 2.0f, 3.0f);
	Vector3 b(0.5f, -1.0f, 2.0f);

	Vector3 sum = a + b;
	REQUIRE(sum == Vector3(1.5f, 1.0f, 5.0f));

	Vector3 diff = a - b;
	REQUIRE(diff == Vector3(0.5f, 3.0f, 1.0f));

	Vector3 scaled = 2.0f * b;
	REQUIRE(scaled == Vector3(1.0f, -2.0f, 4.0f));
}

TEST_CASE("Vector3: Inf() returns infinities") {
	const Vector3 &inf = Vector3::Inf();
	REQUIRE(std::isinf(inf.x));
	REQUIRE(std::isinf(inf.y));
	REQUIRE(std::isinf(inf.z));
}

TEST_CASE("Vector3: ostream prints components") {
	Vector3 v(1.0f, -2.5f, 3.75f);
	std::ostringstream oss;
	oss << v;
	std::string out = oss.str();
	// basic sanity checks on formatting
	REQUIRE(out.front() == '<');
	REQUIRE(out.find(",") != std::string::npos);
	REQUIRE(out.back() == '>');
}
