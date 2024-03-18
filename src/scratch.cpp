#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "vector.h"
#include "color.h"
#include "material.h"
#include "sphere.h"
#include "light.h"
#include "texture.h"
#include <memory>

Vector read_vector(std::string line, float w);

std::vector<Material> materials;
std::vector<std::shared_ptr<Texture>> textures;
std::vector<Sphere> spheres;
std::vector<Light> lights;

// these all have the same length
std::vector<Vector> vertices;
std::vector<Vector> normals;
std::vector<std::vector<float>> uvs;

// these all have the same length and correspond at each index i
std::vector<std::vector<int>> vertex_indices;
std::vector<std::vector<int>> texture_indices;
std::vector<std::vector<int>> normal_indices;
// act like the material field in a sphere but for triangles
std::vector<int> material_indices;

Vector eye_pos;
Vector viewdir;
Vector updir;

float hfov;
int resolution[2];
Color bkgcolor;
float bkgeta;

// parallel projection
float frustum_w = -1.0;
bool parallel = false;

// depth cueing
Color dc = Color(-1, -1, -1);
float alpha[2];
float dist[2];

std::string get_first(std::string in, char delim) {
    int i = 0;
    while (in[i] != delim) {
        i++;
    }
    return in.substr(0, i);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: raytracer1a <name of input file>\n");
        return 0;
    }
    try{
        int read_inputs = 0;
        std::ifstream file(argv[1]);
        std::string line;
        std::string key;
        std::string temp;
        int mtl_index = -1;
        int tex_index = -1;
        bool texture_defined = false;

        while (std::getline(file, line)){
            if (line.empty()) {
                continue;
            }
            key = line.substr(0, line.find(' '));
            if (key == "eye") {
                eye_pos = read_vector(line, 1.0);
                read_inputs++;
            } else if (key == "viewdir") {
                viewdir = read_vector(line, 0.0);
                read_inputs++;
            } else if (key == "updir") {
                updir = read_vector(line, 0.0);
                read_inputs++;
            } else if (key == "hfov") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                ss >> hfov;
                if (!ss || ss >> temp) {
                    throw "Invalid input";
                }
                read_inputs++;
            } else if (key == "imsize") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                ss >> resolution[0] >> resolution[1];
                if (!ss || ss >> temp) {
                    throw "Invalid input";
                }
                read_inputs++;
            } else if (key == "bkgcolor") {
                float r, g, b, eta;
                std::stringstream ss(line.substr(line.find(' ')+1));
                ss >> r >> g >> b >> eta;
                if (!ss || ss >> temp) {
                    throw "Invalid input";
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
                    throw "Invalid input";
                }
                materials.push_back(Material(Color(dr, dg, db), Color(sr, sg, sb), ka, kd, ks, n));
                mtl_index++;
            } else if (key == "texture") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                std::string tex_file;
                ss >> tex_file;
                if (!ss || ss >> temp) {
                    throw "Invalid input";
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
                    throw "Invalid input";
                }
                spheres.push_back(Sphere(Vector(x, y, z, 1.0), r, mtl_index));
            } else if (key == "parallel") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                ss >> frustum_w;
                if (!ss || ss >> temp) {
                    throw "Invalid input";
                }
                parallel = true;
                read_inputs++;
            } else if (key == "light") {
                std::stringstream ss(line.substr(line.find(' ')+1));
                float x, y, z, w, i;
                ss >> x >> y >> z >> w >> i;
                if (!ss || ss >> temp) {
                    throw "Invalid input";
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
                    throw "Invalid input";
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
                        t = "-1";
                    }
                    if (n.empty()) {
                        n = "-1";
                    }
                    vs[i] = std::stoi(v);
                    ts[i] = std::stoi(t);
                    ns[i] = std::stoi(n);
                    i++;
                }
                vertex_indices.push_back(vs);
                texture_indices.push_back(ts);
                normal_indices.push_back(ns);
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
                throw "Invalid input";
            }
        }
    } catch (const char* e) {
        std::cerr << e << std::endl;
        return 1;
    }
    return 0;
}

