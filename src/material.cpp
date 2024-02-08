#include "material.h"

Material::Material() {
    d = Color();
    s = Color(1, 1, 1);
    k[0] = 1.0;
    k[1] = 1.0;
    k[2] = 1.0;
    n_val = 2;
}

Material::Material(Color diffuse, Color specular, float ka, float kd, float ks, int n) {
    d = diffuse;
    s = specular;
    k[0] = ka;
    k[1] = kd;
    k[2] = ks;
    n_val = n;
}

Material::Material(Color diffuse, float ka, float kd, float ks, int n) {
    d = diffuse;
    s = Color(1, 1, 1);
    k[0] = ka;
    k[1] = kd;
    k[2] = ks;
    n_val = n;
}

Material::Material(const Material& m2) {
    d = m2.diffuse();
    s = m2.specular();
    k[0] = m2.ka();
    k[1] = m2.kd();
    k[2] = m2.ks();
    n_val = m2.n();
}

Material::~Material() {
}

Material Material::operator=(const Material& m1) {
    d = m1.diffuse();
    s = m1.specular();
    k[0] = m1.ka();
    k[1] = m1.kd();
    k[2] = m1.ks();
    n_val = m1.n();
    return *this;
}

std::ostream & operator<< ( std::ostream& os, const Material &m1) {
  return os << m1.diffuse() << ", " << m1.specular() << ", " << m1.ka() << ", " << m1.kd() << ", " << m1.ks() << ", " << m1.n();
}