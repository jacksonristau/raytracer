#include <filesystem>
#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <nlohmann/json.hpp>

#include "../include/gfx/scene.h"
#include "../include/gfx/raytracer.h"

using json = nlohmann::json;
using Catch::Approx;

// Most tests use a large "ground" sphere whose top is at (0, 0, -5),
// with a surface point on it lit from above.
const Point3 ground_point(0.0f, 0.0f, -5.0f);

json ground() {
    return { {"center", {0, -100, -5}}, {"radius", 100}, {"material", "opaque"} };
}

json sphere(float x, float y, float z, float r, const std::string& material = "opaque") {
    return { {"center", {x, y, z}}, {"radius", r}, {"material", material} };
}

json point_light(float x, float y, float z) {
    return { {"type", "point"}, {"l", {x, y, z}}, {"i", 1} };
}

json directional_light(float x, float y, float z) {
    return { {"type", "directional"}, {"l", {x, y, z}}, {"i", 1} };
}

json scene_json(json spheres, json lights) {
    return {
        {"camera", {
            {"eye", {0, 0, 0}}, {"view", {0, 0, -1}}, {"up", {0, 1, 0}},
            {"hfov", 60}, {"imsize", {16, 16}}
        }},
        {"bkgcolor", {0, 0, 0}},
        {"materials", {
            { {"name", "opaque"}, {"type", "blinn-phong"}, {"diffuse", {1, 1, 1}}, {"specular", {1, 1, 1}},
              {"ka", 0.2}, {"kd", 0.6}, {"ks", 0.0}, {"n", 10}, {"alpha", 1.0}, {"eta", 1.0} },
            { {"name", "quarter_opaque"}, {"type", "blinn-phong"}, {"diffuse", {1, 1, 1}}, {"specular", {1, 1, 1}},
              {"ka", 0.2}, {"kd", 0.6}, {"ks", 0.0}, {"n", 10}, {"alpha", 0.25}, {"eta", 1.0} },
            { {"name", "half_opaque"}, {"type", "blinn-phong"}, {"diffuse", {1, 1, 1}}, {"specular", {1, 1, 1}},
              {"ka", 0.2}, {"kd", 0.6}, {"ks", 0.0}, {"n", 10}, {"alpha", 0.5}, {"eta", 1.0} }
        }},
        {"spheres", spheres},
        {"lights", lights}
    };
}

// builds the scene and the global BVH the same way raytracer-cli does
Scene load(const json& desc) {
    Scene scene(desc);
    Raytracer::bvh = BVH(Scene::primitives);
    uint32_t next_node = 0;
    Raytracer::bvh.build_bvh_object_median(next_node, 0, Scene::primitives.size());
    return scene;
}

// shoots a ray from `from` at `target` and returns the hit, which must land on `target`
Hit hit_at(const Point3& target, const Point3& from) {
    Hit hit = Raytracer::intersect_scene_bvh(Ray(from, target - from));
    REQUIRE(hit.valid());
    REQUIRE(distance(hit.x_pos, target) < 1e-3f);
    return hit;
}

// comes in diagonally so it never passes through occluders stacked above the ground point
Hit ground_hit() {
    return hit_at(ground_point, Point3(5.0f, 5.0f, -5.0f));
}

float shadow_from_light(const Hit& hit, size_t light = 0) {
    REQUIRE(Scene::lights.size() > light);
    return Raytracer::shadow_factor(hit, Scene::lights[light]);
}

TEST_CASE("shadows: unoccluded point light fully lights the surface") {
    Scene s = load(scene_json({ ground() }, { point_light(0, 10, -5) }));
    CHECK(shadow_from_light(ground_hit()) == Approx(1.0f));
}

TEST_CASE("shadows: opaque occluder between surface and point light blocks it") {
    Scene s = load(scene_json({ ground(), sphere(0, 2, -5, 0.5f) }, { point_light(0, 10, -5) }));
    CHECK(shadow_from_light(ground_hit()) == Approx(0.0f));
}

TEST_CASE("shadows: occluder beyond a point light casts no shadow") {
    // light at y = 1 sits between the surface and the occluder at y = 2
    Scene s = load(scene_json({ ground(), sphere(0, 2, -5, 0.5f) }, { point_light(0, 1, -5) }));
    CHECK(shadow_from_light(ground_hit()) == Approx(1.0f));
}

TEST_CASE("shadows: occluder behind the surface point casts no shadow") {
    // occluder is below the surface, the shadow ray travels up and away from it
    Scene s = load(scene_json({ ground(), sphere(0, -3, -5, 0.5f) }, { point_light(0, 10, -5) }));
    CHECK(shadow_from_light(ground_hit()) == Approx(1.0f));
}

TEST_CASE("shadows: occluder off to the side of the shadow ray casts no shadow") {
    Scene s = load(scene_json({ ground(), sphere(3, 2, -5, 0.5f) }, { point_light(0, 10, -5) }));
    CHECK(shadow_from_light(ground_hit()) == Approx(1.0f));
}

TEST_CASE("shadows: directional light is blocked by occluders at any distance") {
    // light shines straight down, occluder is very far up
    Scene s = load(scene_json({ ground(), sphere(0, 500, -5, 0.5f) }, { directional_light(0, -1, 0) }));
    CHECK(shadow_from_light(ground_hit()) == Approx(0.0f));
}

TEST_CASE("shadows: directional light direction is respected") {
    // light shines straight down, occluder is off to the side
    Scene s = load(scene_json({ ground(), sphere(3, 2, -5, 0.5f) }, { directional_light(0, -1, 0) }));
    CHECK(shadow_from_light(ground_hit()) == Approx(1.0f));
}

TEST_CASE("shadows: only the occluded light is blocked") {
    Scene s = load(scene_json(
        { ground(), sphere(0, 2, -5, 0.5f) },
        { point_light(0, 10, -5), point_light(10, 10, -5) }));
    Hit hit = ground_hit();
    CHECK(shadow_from_light(hit, 0) == Approx(0.0f));
    CHECK(shadow_from_light(hit, 1) == Approx(1.0f));
}

TEST_CASE("shadows: partially transparent occluders let light through") {
    SECTION("one occluder passes 1 - alpha") {
        Scene s = load(scene_json({ ground(), sphere(0, 2, -5, 0.5f, "quarter_opaque") }, { point_light(0, 10, -5) }));
        CHECK(shadow_from_light(ground_hit()) == Approx(0.75f));
    }
    SECTION("stacked occluders multiply") {
        Scene s = load(scene_json(
            { ground(), sphere(0, 2, -5, 0.5f, "quarter_opaque"), sphere(0, 4, -5, 0.5f, "half_opaque") },
            { point_light(0, 10, -5) }));
        CHECK(shadow_from_light(ground_hit()) == Approx(0.75f * 0.5f));
    }
    SECTION("an opaque occluder anywhere on the path fully blocks") {
        Scene s = load(scene_json(
            { ground(), sphere(0, 2, -5, 0.5f, "quarter_opaque"), sphere(0, 4, -5, 0.5f) },
            { point_light(0, 10, -5) }));
        CHECK(shadow_from_light(ground_hit()) == Approx(0.0f));
    }
}

TEST_CASE("shadows: many primitives exercise BVH traversal") {
    // a row of small spheres far from the shadow ray, plus one blocker, so the
    // BVH has several levels and the blocker lives in one leaf among many
    json spheres = { ground() };
    for (int i = 0; i < 40; i++)
        spheres.push_back(sphere(-20.0f + i, 1, -15, 0.2f));
    spheres.push_back(sphere(0, 2, -5, 0.5f));

    Scene s = load(scene_json(spheres, { point_light(0, 10, -5) }));
    REQUIRE(Raytracer::bvh.primitives.size() == 42);
    CHECK(shadow_from_light(ground_hit()) == Approx(0.0f));
}

TEST_CASE("shadows: a curved surface doesn't shadow itself") {
    Scene s = load(scene_json({ sphere(0, 0, -5, 1) }, { point_light(0, 10, -5) }));
    // top of the sphere, and a point partway down its lit side
    Hit top = hit_at(Point3(0, 1, -5), Point3(0, 5, -5));
    CHECK(shadow_from_light(top) == Approx(1.0f));

    Point3 side(0.6f, 0.8f, -5.0f);
    Hit lit_side = hit_at(side, side + 4.0f * Vector3(0.6f, 0.8f, 0.0f));
    CHECK(shadow_from_light(lit_side) == Approx(1.0f));
}

// a quad in the tilted plane y = 0.3x made of two triangles sharing the diagonal
// from (-10, -3, -10) to (10, 3, 10). It's tilted so its bounding box isn't flat;
// a flat box would let the AABB test hide acne that real meshes suffer from.
std::string write_quad_obj() {
    auto path = std::filesystem::temp_directory_path() / "raytracer_shadow_test_quad.obj";
    std::ofstream obj(path);
    obj << "v -10 -3 -10\n"
        << "v  10  3 -10\n"
        << "v  10  3  10\n"
        << "v -10 -3  10\n"
        << "f 1 3 2\n"
        << "f 1 4 3\n";
    return path.string();
}

TEST_CASE("shadows: adjacent mesh triangles don't shadow each other (no shadow acne)") {
    // second light shines along the surface, just above it
    json desc = scene_json(json::array(), { point_light(0, 10, 0), directional_light(-1, -0.32f, 0) });
    desc["meshes"] = { { {"file", write_quad_obj()}, {"material", "opaque"} } };
    Scene s = load(desc);
    REQUIRE(Scene::primitives.size() == 2);

    // points on the shared diagonal edge, at the shared vertex, and inside each triangle
    const Point3 points[] = {
        Point3(0, 0, 0), Point3(3, 0.9f, 3), Point3(-10, -3, -10), Point3(4, 1.2f, -4), Point3(-4, -1.2f, 4)
    };
    for (const Point3& p : points) {
        CAPTURE(p.x, p.z);
        Hit hit = hit_at(p, p + Vector3(0.5f, 5.0f, 0.3f));
        CHECK(shadow_from_light(hit, 0) == Approx(1.0f));
        // grazing light nearly parallel to the surface
        CHECK(shadow_from_light(hit, 1) == Approx(1.0f));
    }
}

TEST_CASE("shadows: shading uses only ambient light in full shadow") {
    // ground material: diffuse 1, ka 0.2, kd 0.6, no specular, no depth cueing
    Point3 eye(5.0f, 5.0f, -5.0f);
    Ray primary(eye, ground_point - eye);

    Scene lit = load(scene_json({ ground() }, { point_light(0, 10, -5) }));
    Color lit_color = Raytracer::trace_ray(primary, 1);
    // light straight overhead: ambient + kd * (n . l) = 0.2 + 0.6
    CHECK(lit_color.r == Approx(0.8f));

    Scene blocked = load(scene_json({ ground(), sphere(0, 2, -5, 0.5f) }, { point_light(0, 10, -5) }));
    Color shadow_color = Raytracer::trace_ray(primary, 1);
    CHECK(shadow_color.r == Approx(0.2f));
    CHECK(shadow_color.g == Approx(0.2f));
    CHECK(shadow_color.b == Approx(0.2f));

    Scene tinted = load(scene_json({ ground(), sphere(0, 2, -5, 0.5f, "quarter_opaque") }, { point_light(0, 10, -5) }));
    Color partial_color = Raytracer::trace_ray(primary, 1);
    CHECK(partial_color.r == Approx(0.2f + 0.75f * 0.6f));
}
