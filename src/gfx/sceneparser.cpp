#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "scene.h"

using json = nlohmann::json;


static Scene load_from_file(const std::string& filename) {
    std::ifstream input;
    std::cout << "loading file: " << filename << std::endl;

    input.open(filename);
    if (!input.is_open()) {
        std::cout << "failed to open file " << filename << std::endl;
        return 0;
    }

    json scene_json;
    try {
        input >> scene_json;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return 0;
    }

    try {

        Point3 eye_pos;
        Vector3 view_dir;
        Vector3 up_dir;
        float hfov;
        int resolution[2];

        auto camera_json = scene_json.at("camera");
        eye_pos = Point3(camera_json.at("eye")[0], camera_json.at("eye")[1], camera_json.at("eye")[2]);
        view_dir = Vector3(camera_json.at("viewdir")[0], camera_json.at("viewdir")[1], camera_json.at("viewdir")[2]);
        view_dir.normalize();
        up_dir = Vector3(camera_json.at("updir")[0], camera_json.at("updir")[1], camera_json.at("updir")[2]);
        hfov = camera_json.at("hfov");
        resolution[0] = camera_json.at("resolution")[0];
        resolution[1] = camera_json.at("resolution")[1];

        auto bkgcolor_json = scene_json.at("bkgcolor");
        bkgcolor = Color(bkgcolor_json[0], bkgcolor_json[1], bkgcolor_json[2]);

        // parse spheres
        for (const auto& sphere_json : scene_json.at("spheres")) {
            Point3 center(sphere_json.at("center")[0], sphere_json.at("center")[1], sphere_json.at("center")[2]);
            float radius = sphere_json.at("radius");
            spheres.push_back(Sphere(center, radius));
        }

        // Parse lights
        for (const auto& light_json : scene_json.at("lights")) {
            if (light_json.at("type") == "point") {
                Point3 l(light_json.at("l")[0], light_json.at("l")[1], light_json.at("l")[2]);
                float i
            }
        }
    }
}