#ifndef COLOR_H
#define COLOR_H

#include <ostream>

class Color {
    public:
        Color();

        Color(float r, float g, float b);

        Color(const Color& p2);

        virtual ~Color();

        // equality
        bool operator==(const Color& p) const;

        Color operator=(const Color& c1);


    private:
        float c[3];
};

std::ostream & operator<< ( std::ostream &os, const Color &p);

#endif