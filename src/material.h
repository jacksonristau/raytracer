#ifndef MATERIAL_H
#define MATERIAL_H

#include <ostream>

class Material {
    public:
        Material();

        Material(float r, float g, float b);

        Material(Color );

        Material(const Material& p2);

        virtual ~Material();

        // equality
        bool operator==(const Material& p) const;

        // negation
        Material operator-();

        Material operator=(const Material& p1);

        float distance(const Material& p1);

    private:
        float p[3];
};

std::ostream & operator<< ( std::ostream &os, const Material &p);

#endif