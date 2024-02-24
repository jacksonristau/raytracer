#ifndef MATERIAL_H
#define MATERIAL_H

#include <ostream>
#include <vector>
#include <string>
#include "color.h"

// holds information for doing blinn-phong lighting calculations
class Material {
    public:
        Material();

        Material(Color diffuse, Color specular, float ka, float kd, float ks, int n);

        Material(std::string filename);

        Material(const Material& m2);

        virtual ~Material();

        Material operator=(const Material& m1);

        static std::vector<std::string> split(std::string in, char delim);
        int load_texture(std::string filename);

        Color diffuse(float u = 0, float v = 0) const;
        Color specular() const {return s;}
        float ka() const {return k[0];}
        float kd() const {return k[1];}
        float ks() const {return k[2];}
        int n() const {return n_val;}

    private:
        Color* texture;
        int width;
        int height;
        Color d;
        Color s;
        float k[3];
        int n_val;
};

std::ostream & operator<< ( std::ostream &os, const Material &m1);

#endif