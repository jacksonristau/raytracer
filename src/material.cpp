#include "material.h"
#include <fstream>
#include <iostream>
#include <cmath>

Material::Material() {
    d = Color();
    s = Color(1, 1, 1);
    k[0] = 0.3;
    k[1] = 0.6;
    k[2] = 0.3;
    n_val = 50;
    texture = -1;
}

Material::Material(Color diffuse, Color specular, float ka, float kd, float ks, int n) {
    d = diffuse;
    s = specular;
    k[0] = ka;
    k[1] = kd;
    k[2] = ks;
    n_val = n;
    texture = -1;
}

Material::Material(int texture) : texture(texture) {
    d = Color();
    s = Color(1, 1, 1);
    k[0] = 0.2;
    k[1] = 0.6;
    k[2] = 0.2;
    n_val = 20;
}

Material::Material(const Material& m2) {
    d = m2.diffuse();
    s = m2.specular();
    k[0] = m2.ka();
    k[1] = m2.kd();
    k[2] = m2.ks();
    n_val = m2.n();
    texture = m2.texture;
}

Material::~Material() {
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



Color Material::diffuse() const {
    return d;
}

Material Material::operator=(const Material& m1) {
    d = m1.diffuse();
    s = m1.specular();
    k[0] = m1.ka();
    k[1] = m1.kd();
    k[2] = m1.ks();
    n_val = m1.n();
    texture = m1.texture;
    return *this;
}

std::ostream & operator<< ( std::ostream& os, const Material &m1) {
  return os << m1.diffuse() << ", " << m1.specular() << ", " << m1.ka() << ", " << m1.kd() << ", " << m1.ks() << ", " << m1.n();
}