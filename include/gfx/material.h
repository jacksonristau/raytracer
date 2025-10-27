#pragma once

#include <ostream>
#include <vector>
#include <string>
#include "color.h"
#include "texture.h"

// holds information for doing blinn-phong lighting calculations
class Material {
    public:
        Material();

        Material(Color diffuse, Color specular, float ka, float kd, float ks, int n, float alpha, float eta);

        Material(int texture);

        Material(const Material& m2);

        virtual ~Material();

        Material operator=(const Material& m1);

        static std::vector<std::string> split(std::string in, char delim);

        float ka() const {return k[0];}
        float kd() const {return k[1];}
        float ks() const {return k[2];}
        int n() const {return n_val;}
        float alpha() const {return a;}
        float eta() const {return index_of_refraction;}
        bool is_glossy() { return k[2] > 0.0f; }
        bool is_transparent() { return a < 1.0f; }

    private:
        Texture* texture;
        float k[3];
        float a;
        float index_of_refraction;
        int n_val;
};


std::ostream & operator<< ( std::ostream &os, const Material &m1);