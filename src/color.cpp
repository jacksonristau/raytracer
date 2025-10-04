#include "color.h"
#include "math/constants.h"
#include "math/floatutil.h"

Color::Color() {
    // nice lil green
    r = 0.2f;
    g = 0.6f;
    b = 0.2f;
}

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

Color::Color(float c1[3]) {
    r = c1[0];
    g = c1[1];
    b = c1[2];
}

Color::Color(const Color& c1) {
    r = c1.r;
    g = c1.g;
    b = c1.b;
}

Color::~Color() {}

// equality
bool Color::operator==(const Color& c1) const {
    return equalf(r, c1.r) && 
            equalf(g, c1.g) &&
            equalf(b, c1.b);
}

bool Color::operator!=(const Color& c1) const {
    return !equalf(r, c1.r) || 
            !equalf(g, c1.g) ||
            !equalf(b, c1.b);
}

Color Color::operator=(const Color& c1) {
    r = c1.r;
    g = c1.g;
    b = c1.b;
    return *this;
}

Color Color::operator+(const Color& c1) {
    return Color(r + c1.r, g + c1.g, b + c1.b);
}

Color operator*(float s, const Color& c1) {
    return Color(s * c1.r, s * c1.g, s * c1.b);
}

Color operator*(const Color& c1, float s){
    return Color(s * c1.r, s * c1.g, s * c1.b);
}

void Color::clamp() {
    if (r > 1.0) r = 1.0f;
    if (g > 1.0) g = 1.0f;
    if (b > 1.0) b = 1.0f;
}

std::ostream & operator<< ( std::ostream& os, const Color &c1) {
  return os << "(" << c1.r << ", " << c1.g << ", " << c1.b << ")";
}