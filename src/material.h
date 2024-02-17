#ifndef MATERIAL_H
#define MATERIAL_H

#include <ostream>
#include "color.h"

// holds information for doing blinn-phong lighting calculations
class Material {
    public:
        Material();

        Material(Color diffuse, Color specular, float ka, float kd, float ks, int n);

        Material(Color diffuse, float ka, float kd, float ks, int n);

        Material(const Material& m2);

        virtual ~Material();

        Material operator=(const Material& m1);

        Color diffuse() const {return d;}
        Color specular() const {return s;}
        float ka() const {return k[0];}
        float kd() const {return k[1];}
        float ks() const {return k[2];}
        int n() const {return n_val;}

    private:
        Color d;
        Color s;
        float k[3];
        int n_val;
};

std::ostream & operator<< ( std::ostream &os, const Material &m1);

#endif