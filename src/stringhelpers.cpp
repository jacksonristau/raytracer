#include <sstream>

#include "../include/stringhelpers.h"


std::vector<std::string> split(std::string in, char delim) {
    std::vector<std::string> out;
    int start = 0;
    int end = 0;
    for (int i = 0; i <= in.size(); i++) {
        if (in[i] == delim || i == in.size()) {
            end = i;
            std::string word = "";
            word.append(in, start, end - start);
            if (!word.empty()) {
                out.push_back(word);
                start = end + 1;
            }
        }
    }
    return out;
}

std::string pixel_to_string(Color pixel) {
    std::stringstream out;
    pixel.clamp();
    out << std::to_string(static_cast<int>(ceil(pixel.r * 255)))
        << " "
        << std::to_string(static_cast<int>(ceil(pixel.g * 255)))
        << " "
        << std::to_string(static_cast<int>(ceil(pixel.b * 255))) << "\n";
    return out.str();
}