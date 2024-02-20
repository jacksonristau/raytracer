#include "scene.h"
#include <fstream>
#include <math.h>

const float pi = 4.0 * atan(1.0);

Scene::Scene() {
    materials = std::vector<Material>();
    objects = std::vector<Sphere>();
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
            out.push_back(word);
            start = end + 1;
        }
    }
    return out;
}

// Static method to load a scene from a file
int Scene::load_from_file(const std::string& filename) {
    std::ifstream input;

    input.open(filename);
    if (!input.is_open()) {
        std::cout << "failed to open file " << filename << std::endl;
        return 0;
    }

    // if theres fewer somethings wrong
    int read_inputs = 0;

    // read values from file
    std::string line;
    int index = -1;
    while(std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }
        std::vector<std::string> keys = split(line, ' ');
        if (keys[0] == "#") {
            continue;
        }
        if (keys[0] == "eye") {
            if (keys.size() != 4) {
                std::cout << "3 numbers are required for eye" << std::endl;
                return 0;
            }
            eye_pos = Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]), 1.0);
            read_inputs++;
        }
        else if (keys[0] == "viewdir") {
            if (keys.size() != 4) {
                std::cout << "3 numbers are required for viewdir" << std::endl;
                return 0;
            }
            viewdir = Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]));
            viewdir.normalize();
            read_inputs++;
        }
        else if (keys[0] == "updir") {
            if (keys.size() != 4) {
                std::cout << "3 numbers are required for updir" << std::endl;
                return 0;
            }
            updir = Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]));
            updir.normalize();
            read_inputs++;
        }
        else if (keys[0] == "hfov") {
            if (keys.size() != 2) {
                std::cout << "1 number is required for hfov" << std::endl;
                return 0;
            }
            hfov = stof(keys[1]) * (pi / 180);
            read_inputs++;
        }
        else if (keys[0] == "imsize") {
            if (keys.size() != 3) {
                std::cout << "2 numbers are required for imsize" << std::endl;
                return 0;
            }
            resolution[0] = stoi(keys[1]);
            resolution[1] = stoi(keys[2]);
            read_inputs++;
        }
        else if (keys[0] == "bkgcolor") {
            if (keys.size() != 4) {
                std::cout << "3 numbers are required for bkgcolor" << std::endl;
                return 0;
            }
            bkgcolor = Color(stof(keys[1]), stof(keys[2]), stof(keys[3]));
            read_inputs++;
        }
        else if (keys[0] == "mtlcolor") {
            if (keys.size() != 11) {
                std::cout << "10 numbers are required for mtlcolor" << std::endl;
                return 0;
            }
            Color d = Color(stof(keys[1]), stof(keys[2]), stof(keys[3]));
            Color s = Color(stof(keys[4]), stof(keys[5]), stof(keys[6]));
            materials.push_back(Material(d, s, stof(keys[7]), stof(keys[8]), stof(keys[9]), stof(keys[10])));
            index++;
        }
        else if (keys[0] == "sphere") {
            if (keys.size() != 5) {
                std::cout << "4 numbers are required for spheres" << std::endl;
                return 0;
            }
            Vector center = Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]), 1.0);
            float radius = stof(keys[4]);
            objects.push_back(Sphere(center, radius, index));
        }
        else if (keys[0] == "parallel") {
            if (keys.size() != 2) {
                std::cout << "1 number is required for parallel" << std::endl;
                return 0;
            }
            frustum_w = stof(keys[1]);
            parallel = true;
            read_inputs++;
        }
        // for the lights the attenuation is set to 1.0, 0.0, 0.0 by default
        else if (keys[0] == "light") {
            if (keys.size() != 6) {
                std::cout << "5 numbers are required for light" << std::endl;
                return 0;
            }
            lights.push_back(Light(Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]), stof(keys[4])), stof(keys[5])));
        }
        else if (keys[0] == "attlight") {
            if (keys.size() != 9) {
                std::cout << "8 numbers are required for attlight" << std::endl;
                return 0;
            }
            Light l = Light(Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]), stof(keys[4])), stof(keys[5]));
            l.set_att(stof(keys[6]), stof(keys[7]), stof(keys[8]));
            lights.push_back(l);
        }
        else if (keys[0] == "depthcueing"){
            if (keys.size() != 8) {
                std::cout << "7 numbers are required for depthcueing" << std::endl;
                return 0;
            }
            dc = Color(stof(keys[1]), stof(keys[2]), stof(keys[3]));
            alpha[0] = stof(keys[4]);
            alpha[1] = stof(keys[5]);
            dist[0] = stof(keys[6]);
            dist[1] = stof(keys[7]);
        }
        else {
            std::cout << "invalid key: "  << keys[0] << std::endl;
            return 0;
        }
    }

    // a few checks to make sure the input is valid
    if (read_inputs < 6) {
        std::cout << "missing a required input: [eye, viewdir, updir, hfov/parallel, res, bkgcolor]" << std::endl;
        return 0;
    }
    else if (index == -1 || objects.size() < 1) {
        std::cout << "missing at least one mtlcolor, and/or one sphere" << std::endl;
        return 0;
    }
    if (parallel && frustum_w <= 0) {
        std::cout << "invalid format: parallel <frustum_width>" << std::endl;
        std::cout << "frustum width must be positive" << std::endl;
        return 0;
    }
    return 1;
}