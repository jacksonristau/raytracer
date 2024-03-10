#include "texture.h"
#include <fstream>
#include <iostream>
#include <cmath>

Texture::Texture(const std::string& filename) : filename(filename) {
    std::ifstream input;
    std::cout << "loading file: " << filename << std::endl;

    input.open(filename);
    if (!input.is_open()) {
        throw std::runtime_error("failed to open texture file " + filename);
    }
    std::string format;
    float max;
    input >> format >> width >> height >> max;
    if (format != "P3" || !input) {
        throw std::runtime_error("invalid texture ppm format");
    }

    data = std::vector<Color>(width * height);
    for (int i = 0; i < width * height; i++) {
        int r, g, b;
        input >> r >> g >> b;
        if (!input) {
            throw std::runtime_error("invalid texture ppm format");
        }
        data[i] = Color(r / max, g / max, b / max);
    }
    input.close();
}

// just for testing if the texture loaded correctly
void Texture::write_to_file(const std::string& filename) const {
    std::ofstream output;
    output.open(filename);
    if (!output.is_open()) {
        throw std::runtime_error("failed to write file " + filename);
    }
    output << "P3\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; i++) {
        output << (int)(data[i].r() * 255) << " " << (int)(data[i].g() * 255) << " " << (int)(data[i].b() * 255) << "\n";
    }
}

Color Texture::get_pixel(float u, float v) const {
    int x, y;
    if (x > 1){
        x = floor(std::modf(u, nullptr) * (width - 1));
    }
    else {
        x = u * (width - 1);
    }
    if (y > 1){
        y = floor(std::modf(v, nullptr) * (height - 1));
    }
    else {
        y = v * (height - 1);
    }
    return data[y * width + x];
}