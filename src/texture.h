#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "color.h"
#include <vector>

class Texture {
public:
        Texture(const std::string& filename);

        Color get_pixel(float u, float v) const;
        int get_width() const { return width; }
        int get_height() const { return height; }

        void write_to_file(const std::string& filename) const;

    private:
        std::string filename;
        int width;
        int height;
        std::vector<Color> data;
};

#endif
