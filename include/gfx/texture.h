#pragma once

#include <string>
#include "color.h"
#include <vector>

class Texture {
    public:
        virtual Color get_pixel(float u, float v) const = 0;
        virtual bool is_uniform() const = 0;
};

class ImageTexture : public Texture {
    public:
        ImageTexture(const std::string& filename);
        Color get_pixel(float u, float v) const;
        bool is_uniform() const { return false; }
        std::string filename;
        int width, height;
        std::vector<Color> data;
};

class BPColorTexture : public Texture {
    public:
        BPColorTexture(Color diffuse) : diffuse(diffuse) {}
        Color get_pixel(float u, float v) const { return diffuse; }
        bool is_uniform() const { return true; }

        Color diffuse;
};