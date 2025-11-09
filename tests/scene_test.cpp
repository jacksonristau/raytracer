#include <iostream>
#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "../include/geometry/sphere.h"
#include "../include/gfx/scene.h"
#include "../include/gfx/camera.h"

using json = nlohmann::json;

std::string test = R"(
{
  "materials": [
    {
      "name": "red",
      "colortexture": [ 0.5, 0, 0 ],
      "specular": [ 0.5, 0, 0 ],
      "ka": 0.2,
      "kd": 0.6,
      "ks": 0.0,
      "n": 50,
      "alpha": 1,
      "eta": 1.2
    }
  ],
  "camera": {
    "eye": [ 0, 0, 0 ],
    "view": [ 0, 0, -1 ],
    "up": [ 0, 1, 0 ],
    "hfov": 90,
    "imsize": [ 800, 600 ],
    "depthcueing": {
      "color": [ 0.5, 0.0, 0.1 ],
      "alpha": [ 0.1, 1 ],
      "dist": [ 1, 10 ]
    }
  },
  "bkgcolor": [ 0.5, 0.5, 0.5 ],
  "spheres": [
    {
      "center": [ 0, 0, -5 ],
      "radius": 1,
      "material": "red"
    }
  ],
  "lights": [
    {
      "l": [ 0, 0, 1 ],
      "type": "point",
      "i": 1,
      "att": [ 0.1, 0.5, 1 ]
    },
    {
      "l": [ 1, 0, 0 ],
      "type": "directional",
      "i": 1
    }
  ]
})";


TEST_CASE("complete scene") {
    json j = json::parse(test);
    Scene s(j);
    REQUIRE(s.camera.up_dir == Vector3(0.0f, 1.0f, 0.0f));
    REQUIRE(s.lights.size() == 2);
    REQUIRE(s.primitives.size() == 1);
    REQUIRE(s.materials.size() == 1);
}