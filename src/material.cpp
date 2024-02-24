#include "material.h"
#include <fstream>
#include <iostream>

Material::Material() {
    d = Color();
    s = Color(1, 1, 1);
    k[0] = 0.3;
    k[1] = 0.6;
    k[2] = 0.3;
    n_val = 50;
}

Material::Material(Color diffuse, Color specular, float ka, float kd, float ks, int n) {
    d = diffuse;
    s = specular;
    k[0] = ka;
    k[1] = kd;
    k[2] = ks;
    n_val = n;
    texture = nullptr;
}

Material::Material(std::string filename) {
    load_texture(filename);
    d = Color();
    s = Color(1, 1, 1);
    k[0] = 0.3;
    k[1] = 0.6;
    k[2] = 0.3;
    n_val = 50;
}

Material::Material(const Material& m2) {
    d = m2.diffuse();
    s = m2.specular();
    k[0] = m2.ka();
    k[1] = m2.kd();
    k[2] = m2.ks();
    n_val = m2.n();
    if (m2.texture != nullptr) {
        texture = new Color[m2.width * m2.height];
        for (int i = 0; i < m2.width * m2.height; i++) {
            texture[i] = m2.texture[i];
        }
    }
    else {
        texture = nullptr;
    }
}

Material::~Material() {
    if (texture != nullptr) {
        delete[] texture;
    }
}

// tokenizes a list by the delimiter
std::vector<std::string> Material::split(std::string in, char delim) {
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

int Material::load_texture(std::string filename) {
    std::ifstream input;
    std::cout << "loading file: " << filename << std::endl;

    input.open(filename);
    if (!input.is_open()) {
        std::cout << "failed to open texture file " << filename << std::endl;
        return 0;
    }
    std::string format;
    float max;
    input >> format >> width >> height >> max;
    if (format != "P3" || !input) {
        std::cout << "invalid texture ppm format" << std::endl;
        return 0;
    }
    try {
        texture = new Color[width * height];
        for (int i = 0; i < width * height; i++) {
            int r, g, b;
            input >> r >> g >> b;
            if (!input) {
                std::cout << "invalid texture ppm format" << std::endl;
                return 0;
            }
            texture[i] = Color(r / max, g / max, b / max);
        }
    }
    catch (std::exception e) {
        std::cout << "invalid texture ppm format" << std::endl;
        return 0;
    }
    return 1;
}

Color Material::diffuse(float u, float v) const {
    if (texture != nullptr) {
        int x = (int)(u * width);
        int y = (int)(v * height);
        return texture[x + y * width];
    }
    return d;
}

Material Material::operator=(const Material& m1) {
    d = m1.diffuse();
    s = m1.specular();
    k[0] = m1.ka();
    k[1] = m1.kd();
    k[2] = m1.ks();
    n_val = m1.n();
    if (m1.texture != nullptr) {
        texture = new Color[m1.width * m1.height];
        for (int i = 0; i < m1.width * m1.height; i++) {
            texture[i] = m1.texture[i];
        }
    }
    else {
        texture = nullptr;
    }
    return *this;
}

std::ostream & operator<< ( std::ostream& os, const Material &m1) {
  return os << m1.diffuse() << ", " << m1.specular() << ", " << m1.ka() << ", " << m1.kd() << ", " << m1.ks() << ", " << m1.n();
}