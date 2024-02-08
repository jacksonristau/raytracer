#include "color.h"
#include <math.h>

Color::Color() {
    // nice lil green
    c[0] = 0.2;
    c[1] = 0.6;
    c[2] = 0.2;
}

Color::Color(float r, float g, float b) {
    c[0] = r;
    c[1] = g;
    c[2] = b;
}

Color::Color(const Color& c2) {
    c[0] = c2.r();
    c[1] = c2.g();
    c[2] = c2.b();
}

Color::~Color() {
}

// equality
bool Color::operator==(const Color& c1) const {
    return fabs(c[0] - c1.r()) < 0.001 && 
            fabs(c[1] - c1.g()) < 0.001 &&
            fabs(c[2] - c1.b()) < 0.001;
}

Color Color::operator=(const Color& c1) {
    c[0] = c1.r();
    c[1] = c1.g();
    c[2] = c1.b();
    return *this;
}

std::ostream & operator<< ( std::ostream& os, const Color &c1) {
  return os << "(" << c1.r() << ", " << c1.g() << ", " << c1.b() << ")";
}