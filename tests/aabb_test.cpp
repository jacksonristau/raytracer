#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <cmath>

#include "../include/math/vector3.h"
#include "../include/math/point3.h"
#include "../include/math/floatutil.h"

#include "../include/geometry/aabb.h"
#include "../include/geometry/sphere.h"

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

TEST_CASE("AABB: bounding primitives") {
	auto sp = std::make_shared<Sphere>(Point3(), 1.0f);
	auto mp = std::make_shared<BPMaterial>(BPMaterial());
	auto sphere_pointer2 = std::make_shared<Sphere>(Point3(), 1.0f);
	auto sphere_pointer3 = std::make_shared<Sphere>(Point3(), 1.0f);

	std::vector<Primitive> primitives = {
		Primitive(sp, mp, 1),   
		Primitive(sp, mp, 2, Transform(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f)))
	};

	AABB bounds = AABB(primitives);

	AABB transformed_bounds = primitives[1].geometry->get_aabb();
	std::cout << "min: " << transformed_bounds.min << " max: " << transformed_bounds.max << '\n';
	REQUIRE(transformed_bounds.max.y == 2);
}
 
