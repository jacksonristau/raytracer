#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <cmath>

#include "../include/math/vector3.h"
#include "../include/math/point3.h"
#include "../include/math/floatutil.h"

constexpr float EPS = 1e-6f;

TEST_CASE("Point3: json constructor") {
	using json = nlohmann::json;
	json j = json::parse(R"(
	  {
		"point": [0.0, 1.0, 0.0]
	  }
	)");
	Point3 p(j.at("point"));
	REQUIRE(p.x == 0.0f);
	REQUIRE(p.y == 1.0f);
	REQUIRE(p.z == 0.0f);
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

	Point3 pneg = -p1;
	REQUIRE(equalf(pneg.x, -p1.x));
	REQUIRE(equalf(pneg.y, -p1.y));
	REQUIRE(equalf(pneg.z, -p1.z));
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
