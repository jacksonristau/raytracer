#include "scene.h"
#include <fstream>
#include <math.h>
#include <sstream>

const float pi = 4.0 * atan(1.0);

Scene::Scene() {
    materials = std::vector<Material>();
    spheres = std::vector<Sphere>();
    lights = std::vector<Light>();
}

Scene::~Scene() {
}

Color Scene::depth_cue(Vector x, Color i, float view_dist) const {
    Color final_color = i;
    if (dc != Color(-1, -1, -1)) {
        float view_dist = eye_pos.distance(x);
        float alpha_dc;
        if (view_dist <= dist[0]) {
            alpha_dc = alpha[1];
        }
        else if (view_dist >= dist[1]) {
            alpha_dc = alpha[0];
        }
        else {
            alpha_dc = alpha[0] + (alpha[1] - alpha[0]) * ((dist[1] - view_dist) / (dist[1] - dist[0]));
        }
        final_color = (alpha_dc * i + (1 - alpha_dc) * dc);
    }
    return final_color;
}

Color Scene::get_texture_color(int index, float u, float v) const {
    if (index == -1) {
        return Color(0, 0, 0);
    }
    return textures[index]->get_pixel(u, v);
}

std::vector<Vector> Scene::get_vertices(int index) const {
    std::vector<Vector> out;
    std::vector<int> indices = vertex_indices[index];
    for (int i = 0; i < 3; i++) {
        out.push_back(vertices[indices[i]]);
    }
    return out;
}

std::vector<Vector> Scene::get_normals(int index) const {
    std::vector<Vector> out;
    std::vector<int> indices = normal_indices[index];
    if (indices[0] == -1) {
        return out;
    }
    for (int i = 0; i < 3; i++) {
        out.push_back(normals[indices[i]]);
    }
    return out;
}

std::vector<std::vector<float>> Scene::get_uvs(int index) const {
    std::vector<std::vector<float>> out;
    std::vector<int> indices = texture_indices[index];
    for (int i = 0; i < 3; i++) {
        out.push_back(uvs[indices[i]]);
    }
    return out;
} 

// tokenizes a list by the delimiter
std::vector<std::string> Scene::split(std::string in, char delim) {
    std::vector<std::string> out;
    int start = 0;
    int end = 0;
    for (int i = 0; i <= in.size(); i++) {
        if (in[i] == delim || i == in.size()) {
            end = i;
            std::string word = "";
            word.append(in, start, end - start);
            if (!word.empty()){
                out.push_back(word);
                start = end + 1;
            }
        }
    }
    return out;
}

Vector read_vector(std::string line, float w) {
    std::stringstream ss(line.substr(line.find(' ')+1));
    float x, y, z;
    std::string temp;
    ss >> x >> y >> z;
    if (!ss || ss >> temp) {
        throw "Invalid input: <vector> <x> <y> <z>";
    }
    return Vector(x, y, z, w);
}


// Static method to load a scene from a file
int Scene::load_from_file(const std::string& filename) {
    std::ifstream input;
    std::cout << "loading file: " << filename << std::endl;

    input.open(filename);
    if (!input.is_open()) {
        std::cout << "failed to open file " << filename << std::endl;
        return 0;
    }

    // if theres fewer somethings wrong
    int read_inputs = 0;

    // read values from file
    std::string line;
    std::string key;
    std::string temp;
    int mtl_index = -1;
    int tex_index = -1;
    bool texture_defined = false;
    try{
        while (std::getline(input, line)){
            if (line.empty()) {
                continue;
            }
            key = line.substr(0, line.find(' '));
            if (key == "#") {
                continue;
            }
            if (key == "eye") {
                eye_pos = read_vector(line, 1.0);
                read_inputs++;
            } else if (key == "viewdir") {
                viewdir = read_vector(line, 0.0);
                viewdir.normalize();
                read_inputs++;
            } else if (key == "updir") {
                updir = read_vector(line, 0.0);
                updir.normalize();
                read_inputs++;
            } else if (key == "hfov") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                ss >> hfov;
                hfov *= (pi / 180);
                if (!ss || ss >> temp) {
                    throw "Invalid input: hfov <angle>";
                }
                read_inputs++;
            } else if (key == "imsize") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                ss >> resolution[0] >> resolution[1];
                if (!ss || ss >> temp) {
                    throw "Invalid input: imsize <width> <height>";
                }
                read_inputs++;
            } else if (key == "bkgcolor") {
                float r, g, b, eta;
                std::stringstream ss(line.substr(line.find(' ')+1));
                ss >> r >> g >> b >> eta;
                if (!ss || ss >> temp) {
                    throw "Invalid input: bkgcolor <r> <g> <b> <eta>";
                }
                bkgcolor = Color(r, g, b);
                bkgeta = eta;
                read_inputs++;
            } else if (key == "mtlcolor") {
                if (texture_defined) {
                    throw "untextured geometry must be defined before textured geometry";
                }
                float dr, dg, db, sr, sg, sb, ka, kd, ks, n, alpha, eta;
                std::stringstream ss(line.substr(line.find(' ')+1));
                ss >> dr >> dg >> db >> sr >> sg >> sb >> ka >> kd >> ks >> n >> alpha >> eta;
                if (!ss || ss >> temp) {
                    throw "Invalid input: mtlcolor <dr> <dg> <db> <sr> <sg> <sb> <ka> <kd> <ks> <n> <alpha> <eta>";
                }
                materials.push_back(Material(Color(dr, dg, db), Color(sr, sg, sb), ka, kd, ks, n, alpha, eta));
                mtl_index++;
            } else if (key == "texture") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                std::string tex_file;
                ss >> tex_file;
                if (!ss || ss >> temp) {
                    throw "Invalid input: texture <filename>";
                }
                textures.push_back(std::make_shared<Texture>(Texture(tex_file)));
                Material texture_mat = Material(materials[mtl_index]);
                texture_mat.set_texture(++tex_index);
                materials.push_back(texture_mat);
                texture_defined = true;
                mtl_index++;
            } else if (key == "sphere") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                float x, y, z, r;
                ss >> x >> y >> z >> r;
                if (!ss || ss >> temp) {
                    throw "Invalid input: sphere <x> <y> <z> <r>";
                }
                spheres.push_back(Sphere(Vector(x, y, z, 1.0), r, mtl_index));
            } else if (key == "parallel") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                ss >> frustum_w;
                if (!ss || ss >> temp) {
                    throw "Invalid input: parallel <frustum_width>";
                }
                parallel = true;
                read_inputs++;
            } else if (key == "light") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                float x, y, z, w, i;
                ss >> x >> y >> z >> w >> i;
                if (!ss || ss >> temp) {
                    throw "Invalid input: light <x> <y> <z> <w> <i>";
                }
                lights.push_back(Light(Vector(x, y, z, w), i));
            } else if (key == "attlight") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                float x, y, z, w, i, a, b, c;
                ss >> x >> y >> z >> w >> i >> a >> b >> c;
                if (!ss || ss >> temp) {
                    throw "Invalid input";
                }
                Light l = Light(Vector(x, y, z, w), i);
                l.set_att(a, b, c);
                lights.push_back(l);
            } else if (key == "depthcueing") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                float r, g, b, alpha_min, alpha_max, dist_min, dist_max;
                ss >> r >> g >> b >> alpha_min >> alpha_max >> dist_min >> dist_max;
                if (!ss || ss >> temp) {
                    throw "Invalid input: depthcueing <r> <g> <b> <alpha_min> <alpha_max> <dist_min> <dist_max>";
                }
                dc = Color(r, g, b);
                alpha[0] = alpha_min;
                alpha[1] = alpha_max;
                dist[0] = dist_min;
                dist[1] = dist_max;
            } else if (key == "v") {
                vertices.push_back(read_vector(line, 1.0));
            } else if (key == "f") {
                std::vector<int> vs(3), ts(3), ns(3);
                std::stringstream ss(line.substr(line.find(' ')+1));
                int i = 0;
                while (getline(ss, temp, ' ')) {
                    std::string v, t, n;
                    std::stringstream ss2(temp);
                    getline(ss2, v, '/');
                    getline(ss2, t, '/');
                    getline(ss2, n, '/');
                    if (t.empty()) {
                        t = "0";
                    }
                    if (n.empty()) {
                        n = "0";
                    }
                    vs[i] = std::stoi(v) - 1;
                    ts[i] = std::stoi(t) - 1;
                    ns[i] = std::stoi(n) - 1;
                    i++;
                }
                vertex_indices.push_back(vs);
                texture_indices.push_back(ts);
                normal_indices.push_back(ns);
                material_indices.push_back(mtl_index);
            } else if (key == "vn") {
                normals.push_back(read_vector(line, 0.0));
            } else if (key == "vt") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                float u, v;
                std::string temp;
                ss >> u >> v;
                if (!ss || ss >> temp) {
                    throw "Invalid input";
                }
                uvs.push_back({u, v});
            } else {
                throw "Invalid input: unrecognized key";
            }
        }
    } catch (const char* e) {
        std::cout << "Error: Invalid argument" << key << std::endl;
        return 0;
    }
    

    // a few checks to make sure the input is valid
    if (read_inputs < 6) {
        std::cout << "missing a required input: [eye, viewdir, updir, hfov/parallel, res, bkgcolor]" << std::endl;
        return 0;
    }
    else if ((mtl_index == -1) || (spheres.size() < 1 && vertex_indices.size() < 1)) {
        std::cout << "missing at least one mtlcolor, and/or one sphere / triangle" << std::endl;
        return 0;
    }
    if (parallel && frustum_w <= 0) {
        std::cout << "invalid format: parallel <frustum_width>" << std::endl;
        std::cout << "frustum width must be positive" << std::endl;
        return 0;
    }
    std::cout << "file loaded successfully" << std::endl;
    return 1;
}