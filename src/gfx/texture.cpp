#include "../include/gfx/texture.h"
#include <fstream>
#include <iostream>
#include <cmath>

ImageTexture::ImageTexture(const std::string& filename) : filename(filename) {
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

Color ImageTexture::get_pixel(float u, float v) const {
    int x, y;
    float temp;
    if (u > 1){
        u = std::modf(u, &temp);
    }
    if (v > 1){
        v = std::modf(v, &temp);
    }
    y = static_cast<int>((1.0f - v) * (height - 1));
    x = static_cast<int>(u * (width - 1));
    if (y * width + x >= data.size()){
        throw std::runtime_error("invalid texture coordinates: " + std::to_string(x) + ", " + std::to_string(y) + " in " + filename + " with size " + std::to_string(width) + "x" + std::to_string(height) + " and " + std::to_string(data.size()) + " pixels");
    }
    return data[y * width + x];
}

