#include <iostream>
#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "../include/geometry/sphere.h"

using json = nlohmann::json;


TEST_CASE("complete scene") {
    std::ifstream i("../../files/example.json");
    json j;
    i >> j;
    json spheres = j.at("spheres");
    Sphere s (spheres[0]);
    REQUIRE(s.radius == 1);
    REQUIRE(s.center == Point3(0.0f, 0.0f, -5.0f));
}