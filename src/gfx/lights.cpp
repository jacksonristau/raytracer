#include "../include/gfx/lights.h"
#include <string>

ILight* LightFactory::create(const json& light_json) {
    if (!light_json.contains("type")) {
        return nullptr;
    }
    std::string type = light_json.at("type");
    if (type == "directional") {
        return new DirectionalLight(light_json);
    }
    else if (type == "point") {
        return new PointLight(light_json);
    }
    // unknown type
    return nullptr;
}

PointLight::PointLight(json light_json){
    l = Point3(0.0f, 0.0f, 10.0f);
    try {
        if (light_json.contains("l")) l = l = Point3(light_json.at("l"));
        if (light_json.contains("i")) i = light_json.at("i");

        if (light_json.contains("att") && light_json.at("att").is_array()) {
            c0 = light_json.at("att")[0];
            c1 = light_json.at("att")[1];
            c2 = light_json.at("att")[2];
        }
    }
    catch (const json::exception& e) {
        std::cerr << "Warning: malformed light json: " << e.what() << std::endl;
    }
}

DirectionalLight::DirectionalLight(json light_json) {
    l = Vector3(0.0f, 0.0f, -1.0f);
    try {
        if (light_json.contains("l")) l = l = Vector3(light_json.at("l"));
        if (light_json.contains("i")) i = light_json.at("i");
    }
    catch (const json::exception& e) {
        std::cerr << "Warning: malformed light json: " << e.what() << std::endl;
    }
}