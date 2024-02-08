#include "color.h"

Color::Color() {
    c[0] = 0.0;
    c[1] = 0.0;
    c[2]
}

Color(float r, float g, float b);

Color(const Color& p2);

virtual ~Color();

// equality
bool operator==(const Color& p) const;

Color operator=(const Color& c1);