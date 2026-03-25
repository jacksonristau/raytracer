#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <cmath>

#include "../include/math/vector3.h"
#include "../include/math/point3.h"
#include "../include/math/floatutil.h"

#include "../include/geometry/aabb.h"
#include "../include/geometry/sphere.h"
#include "../include/geometry/primitive.h"

#include "../include/gfx/material.h"

TEST_CASE("AABB: basic constructors") {
	AABB box1 = AABB();
	REQUIRE((box1.min.x == 0 && box1.max.x == 0));
	REQUIRE((box1.min.y == 0 && box1.max.y == 0));
	REQUIRE((box1.min.z == 0 && box1.max.z == 0));

	AABB box2 = AABB(Point3(), Point3(1.0f, 1.0f, 1.0f));
	REQUIRE((box2.min.x == 0 && box2.max.x == 1.0f));
	REQUIRE((box2.min.y == 0 && box2.max.y == 1.0f));
	REQUIRE((box2.min.z == 0 && box2.max.z == 1.0f));
}

TEST_CASE("AABB: combine") {
	AABB box1(Point3(0,0,0), Point3(1,1,1));
	AABB box2(Point3(2,2,2), Point3(3,3,3));
	AABB combined = box1.combine(box2);

	REQUIRE(equalf(combined.min.x, 0.0f));
	REQUIRE(equalf(combined.min.y, 0.0f));
	REQUIRE(equalf(combined.min.z, 0.0f));
	REQUIRE(equalf(combined.max.x, 3.0f));
	REQUIRE(equalf(combined.max.y, 3.0f));
	REQUIRE(equalf(combined.max.z, 3.0f));
}

TEST_CASE("AABB: center") {
	AABB box1(Point3(-2,-2,-2), Point3(2,2,2));
	Point3 c1 = box1.center();
	REQUIRE(equalf(c1.x, 0.0f));
	REQUIRE(equalf(c1.y, 0.0f));
	REQUIRE(equalf(c1.z, 0.0f));

	AABB box2(Point3(0,0,0), Point3(4,2,6));
	Point3 c2 = box2.center();
	REQUIRE(equalf(c2.x, 2.0f));
	REQUIRE(equalf(c2.y, 1.0f));
	REQUIRE(equalf(c2.z, 3.0f));
}

TEST_CASE("AABB: intersect hit") {
	// box from (0,0,0) to (4,4,4), ray approaching from (-1,-1,-1) along (1,1,1)
	AABB box(Point3(0,0,0), Point3(4,4,4));
	Ray ray(Point3(-1,-1,-1), Vector3(1,1,1));
	REQUIRE(box.intersect(ray) >= 0);
}

TEST_CASE("AABB: intersect miss") {
	// ray moving along (1,1,1) but offset far above the box in y — misses
	AABB box(Point3(0,0,0), Point3(2,2,2));
	Ray ray(Point3(-3,10,0), Vector3(1,1,1));
	REQUIRE(box.intersect(ray) == -1);
}

TEST_CASE("AABB: intersect behind") {
	// box is entirely behind the ray origin
	AABB box(Point3(0,0,0), Point3(2,2,2));
	Ray ray(Point3(10,1,1), Vector3(1,1,1));
	REQUIRE(box.intersect(ray) == -1);
}

TEST_CASE("AABB: primitive get_aabb identity") {
	auto sp = std::make_shared<Sphere>(Point3(0,0,0), 1.0f);
	auto mp = std::make_shared<BPMaterial>(BPMaterial());
	Primitive p(sp, mp, 1);

	AABB bounds = p.get_aabb();
	REQUIRE(equalf(bounds.min.x, -1.0f));
	REQUIRE(equalf(bounds.min.y, -1.0f));
	REQUIRE(equalf(bounds.min.z, -1.0f));
	REQUIRE(equalf(bounds.max.x,  1.0f));
	REQUIRE(equalf(bounds.max.y,  1.0f));
	REQUIRE(equalf(bounds.max.z,  1.0f));
}

TEST_CASE("AABB: primitive get_aabb translation") {
	// sphere at origin radius 1, translated (3,0,0) -> world AABB (2,-1,-1) to (4,1,1)
	auto sp = std::make_shared<Sphere>(Point3(0,0,0), 1.0f);
	auto mp = std::make_shared<BPMaterial>(BPMaterial());
	Transform t(Vector3(3,0,0), Vector3(0,0,0), Vector3(1,1,1));
	Primitive p(sp, mp, 1, t);

	AABB bounds = p.get_aabb();
	REQUIRE(equalf(bounds.min.x, 2.0f));
	REQUIRE(equalf(bounds.max.x, 4.0f));
	REQUIRE(equalf(bounds.min.y, -1.0f));
	REQUIRE(equalf(bounds.max.y,  1.0f));
}

TEST_CASE("AABB: primitive get_aabb scale") {
	// sphere at origin radius 1, scaled (2,3,4) -> world AABB (-2,-3,-4) to (2,3,4)
	auto sp = std::make_shared<Sphere>(Point3(0,0,0), 1.0f);
	auto mp = std::make_shared<BPMaterial>(BPMaterial());
	Transform t(Vector3(0,0,0), Vector3(0,0,0), Vector3(2,3,4));
	Primitive p(sp, mp, 1, t);

	AABB bounds = p.get_aabb();
	REQUIRE(equalf(bounds.min.x, -2.0f));
	REQUIRE(equalf(bounds.max.x,  2.0f));
	REQUIRE(equalf(bounds.min.y, -3.0f));
	REQUIRE(equalf(bounds.max.y,  3.0f));
	REQUIRE(equalf(bounds.min.z, -4.0f));
	REQUIRE(equalf(bounds.max.z,  4.0f));
}

TEST_CASE("AABB: primitive get_aabb rotation") {
	// sphere at origin, non-uniform scale (2,1,1) then 90 deg rotation around Z
	// local AABB after scale: (-2,-1,-1) to (2,1,1)
	// after 90 deg Z rotation (x->-y, y->x): x range [-1,1], y range [-2,2], z range [-1,1]
	auto sp = std::make_shared<Sphere>(Point3(0,0,0), 1.0f);
	auto mp = std::make_shared<BPMaterial>(BPMaterial());
	Transform t(Vector3(0,0,0), Vector3(0,0,90), Vector3(2,1,1));
	Primitive p(sp, mp, 1, t);

	AABB bounds = p.get_aabb();
	REQUIRE(equalf(bounds.min.x, -1.0f));
	REQUIRE(equalf(bounds.max.x,  1.0f));
	REQUIRE(equalf(bounds.min.y, -2.0f));
	REQUIRE(equalf(bounds.max.y,  2.0f));
	REQUIRE(equalf(bounds.min.z, -1.0f));
	REQUIRE(equalf(bounds.max.z,  1.0f));
}