#pragma once
#include <ostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Color {
    public:
        Color();

        Color(float r, float g, float b);

        Color(float c2[3]);

        Color(json color);

        Color(const Color& c1);

        virtual ~Color();

        // equality
        bool operator==(const Color& c1) const;

        bool operator!=(const Color& c1) const;

        Color operator=(const Color& c1);

        Color operator+(const Color& c1);

        void clamp();

        float r, g, b;
};

Color operator*(float s, const Color& c1);

Color operator*(const Color& c1, float s);

std::ostream & operator<< ( std::ostream &os, const Color &c1);