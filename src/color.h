#ifndef COLOR_H
#define COLOR_H

#include <ostream>

class Color {
    public:
        Color();

        Color(float r, float g, float b);

        Color(float c[3]);

        Color(const Color& c1);

        virtual ~Color();

        // equality
        bool operator==(const Color& c1) const;

        bool operator!=(const Color& c1) const;

        Color operator=(const Color& c1);

        Color operator+(const Color& c1);

        float r() const {return c[0];}
        float g() const {return c[1];}
        float b() const {return c[2];}

        void clamp();

    private:
        float c[3];
};

Color operator*(float s, const Color& c1);

std::ostream & operator<< ( std::ostream &os, const Color &c1);

#endif