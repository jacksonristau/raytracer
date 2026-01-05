#include <fstream>
#include <sstream>
#include <iostream>

#include "../include/math/floatutil.h"
#include "../include/gfx/lights.h"
#include "../include/gfx/scene.h"
#include "../include/geometry/triangle.h"
#include "../include/geometry/sphere.h"
#include "../include/gfx/tiny_obj_loader.h"
#include "../include/geometry/boundingbox.h"


#include <nlohmann/json.hpp>
using json = nlohmann::json;

Camera Scene::camera;
std::vector<ILight*> Scene::lights;
std::vector<Primitive> Scene::primitives;
std::vector<Mesh> Scene::meshes;

Scene::Scene() {
    materials = std::vector<std::shared_ptr<Material>>();
    lights = std::vector<ILight*>();
    primitives = std::vector<Primitive>();
    meshes = std::vector<Mesh>();
    attribs = std::vector<tinyobj::attrib_t>();
    camera = Camera();
    bkgcolor = Color();
}

 Scene::Scene(json scene_desc) {
    std::unordered_map<std::string, int> material_map;
    bkgcolor = Color();
    if (scene_desc.contains("bkgcolor")){
        bkgcolor = Color(scene_desc.at("bkgcolor"));
    }
    bkgeta = 1.000293;
    if (scene_desc.contains("bkgeta")){
        bkgeta = scene_desc.at("bkgeta");
    }
    if (!scene_desc.contains("camera")) {
        std::cout << "camera settings must be provided, see example.json" << '\n';
        throw std::runtime_error("Failed to create scene, no camera was provided");
    }
    camera = Camera(scene_desc.at("camera"));

    lights = std::vector<ILight*>();
    if (!scene_desc.contains("lights")) {
        std::cout << "warning: no lights provided" << '\n';
    }
    else {
        for (const json& light_json : scene_desc.at("lights")) {
            ILight* light = LightFactory::create(light_json);
            if (light) lights.push_back(light);
        }
    }
         
    materials = std::vector<std::shared_ptr<Material>>();
    if (!scene_desc.contains("materials")) {
        std::cout << "warning: no materials provided" << '\n';
    }
    else {
        int i = 0;
        for (const json& material_json : scene_desc.at("materials")) {
            auto m = std::make_shared<BPMaterial>(material_json);
            materials.push_back(m);
            std::string name = material_json.at("name");
            material_map.insert(std::make_pair(name, i++));
        }
    }

    primitives = std::vector<Primitive>();
    int id = 0;
    if (scene_desc.contains("spheres")) {
        for (const json& sphere_json : scene_desc.at("spheres")) {
			auto s = std::make_shared<Sphere>(sphere_json);
			std::shared_ptr<Material> m;
            if (!sphere_json.contains("material")) {
                if (!materials.empty()) {
					m = materials[0];
                }
                else {
					m = std::make_shared<BPMaterial>();
                    materials.push_back(m);
                }
            }
            else {
                std::string name = sphere_json.at("material");
                if (material_map.count(name) > 0) {
                    m = materials[material_map.at(name)];
                }
                else {
                    throw std::runtime_error("Failed to create scene, unknown material: " +  name);
                }
            }

            // Parse transform if present
            Transform transform;
            if (sphere_json.contains("transform")) {
                transform = Transform(sphere_json["transform"]);
            }

			primitives.emplace_back(s, m, id++, transform);
        }
    }
    meshes = std::vector<Mesh>();
    if (scene_desc.contains("meshes")) {
        for (json & mesh_json : scene_desc.at("meshes")) {
            tinyobj::ObjReaderConfig reader_config;
            reader_config.mtl_search_path = "./";
            tinyobj::ObjReader reader;
            std::string file = mesh_json.at("file");

            if (!reader.ParseFromFile(file, reader_config)) {
                if (!reader.Error().empty()) {
                    std::cerr << "tinyobj: " << reader.Error();
                }
                throw reader.Error();
            }

            if (!reader.Warning().empty()) {
                std::cout << "tinyobj: " << reader.Warning();
            }
            
            std::cout << "building primitive list\n";
            for (const auto& shape : reader.GetShapes()) {
                auto attrib_ptr = std::make_shared<tinyobj::attrib_t>(reader.GetAttrib());
                meshes.emplace_back(attrib_ptr, shape);
                Transform mesh_transform;
                if (mesh_json.contains("transform")) {
                    json transform_json = mesh_json["transform"];
                    Vector3 translation = Vector3(transform_json["translate"]);
                    Vector3 scale = Vector3(transform_json["scale"]);
                    Vector3 rotate = Vector3(transform_json["rotate"]);
                    BoundingBox bb = BoundingBox(meshes.back());
                    Point3 center_offset = bb.center();
                    Vector3 centered_translation = Vector3(-center_offset.x * scale.x, 
                                                           -center_offset.y * scale.y, 
                                                           -center_offset.z * scale.z);
                    translation = translation + centered_translation;
                    std::cout << "BB Min: " << bb.min.x << ", " << bb.min.y << ", " << bb.min.z << std::endl;
                    std::cout << "BB Max: " << bb.max.x << ", " << bb.max.y << ", " << bb.max.z << std::endl;
                    std::cout << "BB Center: " << bb.center().x << ", " << bb.center().y << ", " << bb.center().z << std::endl;
                    std::cout << "Centered Translation: " << centered_translation.x << ", " << centered_translation.y << ", " << centered_translation.z << std::endl;
                    std::cout << "Final Translation: " << translation.x << ", " << translation.y << ", " << translation.z << std::endl;                    
                    mesh_transform = Transform(translation, rotate, scale);
                }
                //Vector3 translation = Point3(0, 0, 0) - bb.center();
                // Create primitives for this shape's triangles with per-face materials
                for (size_t i = 0; i < meshes.back().ntris; i++) {
                    std::shared_ptr<Material> m;

                    // Look up material for this face from OBJ file's material_ids
                    if (!shape.mesh.material_ids.empty() && i < shape.mesh.material_ids.size()) {
                        int mat_id = shape.mesh.material_ids[i];
                        if (mat_id >= 0 && mat_id < static_cast<int>(reader.GetMaterials().size())) {
                            std::string mat_name = reader.GetMaterials()[mat_id].name;
                            if (material_map.count(mat_name) > 0) {
                                m = materials[material_map.at(mat_name)];
                            }
                            else {
                                std::cerr << "warning: OBJ material '" << mat_name << "' not found in scene materials, using default\n";
                                if (!materials.empty()) {
                                    m = materials[0];
                                }
                                else {
                                    m = std::make_shared<BPMaterial>();
                                    materials.push_back(m);
                                }
                            }
                        }
                        else {
                            // Invalid material ID, use default
                            if (!materials.empty()) {
                                m = materials[0];
                            }
                            else {
                                m = std::make_shared<BPMaterial>();
                                materials.push_back(m);
                            }
                        }
                    }
                    else {
                        // No material_ids available, use default
                        if (!materials.empty()) {
                            m = materials[0];
                        }
                        else {
                            m = std::make_shared<BPMaterial>();
                            materials.push_back(m);
                        }
                    }

                    auto tri = std::make_shared<Triangle>(&meshes.back(), int(i * 3));
                    primitives.emplace_back(tri, m, id++, mesh_transform);
                }
            }
        }
    }
}

 Scene::~Scene() {}
