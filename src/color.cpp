#include "color.h"

Color::Color() {
    // nice lil green
    c[0] = 0.2f;
    c[1] = 0.6f;
    c[2] = 0.2f;
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

Color::Color(float c2[3]) {
    c[0] = c2[0];
    c[1] = c2[1];
    c[2] = c2[2];
}

Color::~Color() {
}

// equality
bool Color::operator==(const Color& c1) const {
    return fabs(c[0] - c1.r()) < 0.001f && 
            fabs(c[1] - c1.g()) < 0.001f &&
            fabs(c[2] - c1.b()) < 0.001f;
}

bool Color::operator!=(const Color& c1) const {
    return fabs(c[0] - c1.r()) > 0.01f || 
            fabs(c[1] - c1.g()) > 0.01f ||
            fabs(c[2] - c1.b()) > 0.01f;
}

Color Color::operator=(const Color& c1) {
    c[0] = c1.r();
    c[1] = c1.g();
    c[2] = c1.b();
    return *this;
}

Color Color::operator+(const Color& c1) {
    return Color(c[0] + c1.r(), c[1] + c1.g(), c[2] + c1.b());
}

Color operator*(float s, const Color& c1) {
    return Color(s * c1.r(), s * c1.g(), s * c1.b());
}

void Color::clamp() {
    if (c[0] > 1.0) c[0] = 1.0f;
    if (c[1] > 1.0) c[1] = 1.0f;
    if (c[2] > 1.0) c[2] = 1.0f;
}

std::ostream & operator<< ( std::ostream& os, const Color &c1) {
  return os << "(" << c1.r() << ", " << c1.g() << ", " << c1.b() << ")";
}