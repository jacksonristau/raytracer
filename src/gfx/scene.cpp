#include <fstream>
#include <sstream>
#include <iostream>

#include "../include/math/floatutil.h"
#include "../include/gfx/lights.h"
#include "../include/gfx/scene.h"
#include "../include/geometry/triangle.h"
#include "../include/geometry/sphere.h"
#include "../include/gfx/tiny_obj_loader.h"


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
    bkgcolor = scene_desc.contains("bkgcolor") ? Color(scene_desc.at("bkgcolor")) : Color();
    bkgeta = scene_desc.contains("bkgeta") ? scene_desc.at("bkgeta") : 1.000293;
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
			primitives.emplace_back(s, m, id++);
        }
    }
    meshes = std::vector<Mesh>();
    attribs = std::vector<tinyobj::attrib_t>();
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
            attribs.push_back(reader.GetAttrib());
            for (const auto& shape : reader.GetShapes()) {
                meshes.emplace_back(attribs.back(), shape);
            }
            for (size_t i = 0; i < meshes.back().ntris; i++) {
                std::shared_ptr<Material> m;
                if (!mesh_json.contains("material")) {
                    if (!materials.empty()) {
                        m = materials[0];
                    }
                    else {
                        m = std::make_shared<BPMaterial>();
                        materials.push_back(m);
                    }
                }
                else {
                    std::string name = mesh_json.at("material");
                    if (material_map.count(name) > 0) {
                        m = materials[material_map.at(name)];
                    }
                    else {
                        throw std::runtime_error("Failed to create scene, unknown material: " + name);
                    }
                }
                auto tri = std::make_shared<Triangle>(&meshes.back(), int(i * 3));
                primitives.emplace_back(tri, m, id++);
            }
        }
    }
 }


Scene::~Scene() {}

// Vector3 read_vector(std::string line) {
//     std::stringstream ss(line.substr(line.find(' ')+1));
//     float x, y, z;
//     std::string temp;
//     ss >> x >> y >> z;
//     if (!ss || ss >> temp) {
//         throw "Invalid input: <vector> <x> <y> <z>";
//     }
//     return Vector3(x, y, z);
// }

// Point3 read_point(std::string line) {
//     std::stringstream ss(line.substr(line.find(' ')+1));
//     float x, y, z;
//     std::string temp;
//     ss >> x >> y >> z;
//     if (!ss || ss >> temp) {
//         throw "Invalid input: <vector> <x> <y> <z>";
//     }
//     return Point3(x, y, z);
// }

// Static method to load a scene from a file
// int Scene::load_from_file(const std::string& filename) {
//     std::ifstream input;
//     std::cout << "loading file: " << filename << std::endl;

//     input.open(filename);
//     if (!input.is_open()) {
//         std::cout << "failed to open file " << filename << std::endl;
//         return 0;
//     }

//     // if theres fewer somethings wrong
//     int read_inputs = 0;

//     // read values from file
//     std::string line;
//     std::string key;
//     std::string temp;
//     int mtl_index = -1;
//     int tex_index = -1;
//     bool texture_defined = false;
//     Point3 eye_pos;
//     Vector3 view_dir;
//     Vector3 up_dir;
//     float hfov;
//     int resolution[2];
//     float frustum_w = -1.0f;
//     bool parallel = false;
//     Color dc = Color(-1, -1, -1);
//     float alpha[2];
//     float dist[2];
//     try{
//         while (std::getline(input, line)){
//             if (line.empty()) {
//                 continue;
//             }
//             key = line.substr(0, line.find(' '));
//             if (key == "#") {
//                 continue;
//             }
//             if (key == "eye") {
//                 eye_pos = read_point(line);
//                 read_inputs++;
//             } else if (key == "viewdir") {
//                 view_dir = read_vector(line);
//                 view_dir.normalize();
//                 read_inputs++;
//             } else if (key == "updir") {
//                 up_dir = read_vector(line);
//                 up_dir.normalize();
//                 read_inputs++;
//             } else if (key == "hfov") {
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 ss >> hfov;
//                 hfov *= (pi / 180.0f);
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input: hfov <angle>";
//                 }
//                 read_inputs++;
//             } else if (key == "imsize") {
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 ss >> resolution[0] >> resolution[1];
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input: imsize <width> <height>";
//                 }
//                 read_inputs++;
//             } else if (key == "bkgcolor") {
//                 float r, g, b, eta;
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 ss >> r >> g >> b >> eta;
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input: bkgcolor <r> <g> <b> <eta>";
//                 }
//                 bkgcolor = Color(r, g, b);
//                 bkgeta = eta;
//                 read_inputs++;
//             } else if (key == "mtlcolor") {
//                 if (texture_defined) {
//                     throw "untextured geometry must be defined before textured geometry";
//                 }
//                 int n;
//                 float dr, dg, db, sr, sg, sb, ka, kd, ks, alpha, eta;
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 ss >> dr >> dg >> db >> sr >> sg >> sb >> ka >> kd >> ks >> n >> alpha >> eta;
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input: mtlcolor <dr> <dg> <db> <sr> <sg> <sb> <ka> <kd> <ks> <n> <alpha> <eta>";
//                 }
//                 materials.push_back(Material(Color(dr, dg, db), Color(sr, sg, sb), ka, kd, ks, n, alpha, eta));
//                 mtl_index++;
//             } else if (key == "texture") {
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 std::string tex_file;
//                 ss >> tex_file;
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input: texture <filename>";
//                 }
//                 textures.push_back(std::make_shared<Texture>(Texture(tex_file)));
//                 Material texture_mat = Material(materials[mtl_index]);
//                 texture_mat.set_texture(++tex_index);
//                 materials.push_back(texture_mat);
//                 texture_defined = true;
//                 mtl_index++;
//             } else if (key == "sphere") {
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 float x, y, z, r;
//                 ss >> x >> y >> z >> r;
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input: sphere <x> <y> <z> <r>";
//                 }
//                 spheres.push_back(Sphere(Point3(x, y, z), r, mtl_index));
//             } else if (key == "parallel") {
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 ss >> frustum_w;
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input: parallel <frustum_width>";
//                 }
//                 parallel = true;
//                 read_inputs++;
//             } else if (key == "light") {
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 float x, y, z, w, i;
//                 ss >> x >> y >> z >> w >> i;
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input: light <x> <y> <z> <w> <i>";
//                 }
//                 if (equalf(w, 1.0)){
//                     lights.push_back(new LightP(Point3(x, y, z), i));
//                 }
//                 else {
//                     lights.push_back(new LightD(Vector3(x, y, z), i));
//                 }
//             } else if (key == "attlight") {
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 float x, y, z, w, i, c0, c1, c2;
//                 ss >> x >> y >> z >> w >> i >> c0 >> c1 >> c2;
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input";
//                 }
//                 if (equalf(w, 1.0)){
//                     lights.push_back(new LightP(Point3(x, y, z), i, c0, c1, c2));
//                 }
//                 else {
//                     lights.push_back(new LightD(Vector3(x, y, z), i, c0, c1, c2));
//                 }
//             } else if (key == "depthcueing") {
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 float r, g, b, alpha_min, alpha_max, dist_min, dist_max;
//                 ss >> r >> g >> b >> alpha_min >> alpha_max >> dist_min >> dist_max;
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input: depthcueing <r> <g> <b> <alpha_min> <alpha_max> <dist_min> <dist_max>";
//                 }
//                 dc = Color(r, g, b);
//                 alpha[0] = alpha_min;
//                 alpha[1] = alpha_max;
//                 dist[0] = dist_min;
//                 dist[1] = dist_max;
//             } else if (key == "v") {
//                 vertices.push_back(read_point(line));
//             } else if (key == "f") {
//                 std::vector<int> vs(3), ts(3), ns(3);
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 int i = 0;
//                 while (getline(ss, temp, ' ')) {
//                     std::string v, t, n;
//                     std::stringstream ss2(temp);
//                     getline(ss2, v, '/');
//                     getline(ss2, t, '/');
//                     getline(ss2, n, '/');
//                     if (t.empty()) {
//                         t = "0";
//                     }
//                     if (n.empty()) {
//                         n = "0";
//                     }
//                     vs[i] = std::stoi(v) - 1;
//                     ts[i] = std::stoi(t) - 1;
//                     ns[i] = std::stoi(n) - 1;
//                     i++;
//                 }
//                 vertex_indices.push_back(vs);
//                 texture_indices.push_back(ts);
//                 normal_indices.push_back(ns);
//                 material_indices.push_back(mtl_index);
//             } else if (key == "vn") {
//                 normals.push_back(read_vector(line));
//             } else if (key == "vt") {
//                 std::stringstream ss(line.substr(line.find(' ')+1));
//                 float u, v;
//                 std::string temp;
//                 ss >> u >> v;
//                 if (!ss || ss >> temp) {
//                     throw "Invalid input";
//                 }
//                 uvs.push_back({u, v});
//             } else {
//                 throw "Invalid input: unrecognized key";
//             }
//         }
//     } catch (const char* e) {
//         std::cout << "Error: Invalid argument" << key << '\n' << e << '\n';
//         return 0;
//     }
    

//     // a few checks to make sure the input is valid
//     if (read_inputs < 6) {
//         std::cout << "missing a required input: [eye, viewdir, updir, hfov/parallel, res, bkgcolor]" << std::endl;
//         return 0;
//     }
//     else if ((mtl_index == -1) || (spheres.size() < 1 && vertex_indices.size() < 1)) {
//         std::cout << "missing at least one mtlcolor, and/or one sphere / triangle" << std::endl;
//         return 0;
//     }
//     if (parallel && frustum_w <= 0) {
//         std::cout << "invalid format: parallel <frustum_width>" << std::endl;
//         std::cout << "frustum width must be positive" << std::endl;
//         return 0;
//     }
//     // viewdir cross updir approaching invalidity with fp error
//     float vdotu = view_dir.dot(up_dir);
//     if (vdotu < -0.9 || vdotu > 0.9) {
//         std::cout << "up vector is too close to view vector" << std::endl;
//         return 0;
//     }
//     camera = Camera(resolution, hfov, dc, alpha, dist, eye_pos, view_dir, up_dir);
//     std::cout << "file loaded successfully" << std::endl;
//     return 1;
// }