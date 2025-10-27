#pragma once

#include <string>
#include "color.h"
#include <vector>

class Texture {
    public:
        Texture();

        virtual Color get_pixel(float u, float v) const = 0;

};

class ImageTexture {
    public:
        ImageTexture(const std::string& filename);
        Color get_pixel(float u, float v) const;

        std::string filename;
        int width, height;
    private:
        std::vector<Color> data;
};

class BPColorTexture {
    public:
        BPColorTexture(Color diffuse, Color specular);
        Color get_pixel(float u, float v) const;

        Color diffuse, specular;
};