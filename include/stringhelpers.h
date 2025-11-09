#include <vector>
#include <string>
#include "gfx/color.h"

// tokenizes a list by the delimiter
std::vector<std::string> split(std::string in, char delim);

// convert from a Color struct to a string
std::string pixel_to_string(Color pixel);