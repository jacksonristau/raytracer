#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include "math/vector3.h"
#include <vector>

typedef struct {
    float r, g, b;
} Color;

typedef struct {
    float x, y, z;
    float dx, dy, dz;
} Ray;

typedef struct {
    float x, y, z;
    float r;
    int m;
} Sphere;

std::vector<std::string> split(std::string in, char delim) {
    std::vector<std::string> out;
    int start = 0;
    int end = 0;
    for (int i = 0; i <= in.size(); i++) {
        if (in[i] == delim || i == in.size()) {
            end = i;
            std::string word = "";
            word.append(in, start, end - start);
            out.push_back(word);
            start = end + 1;
        }
    }
    return out;
}

int main(int argc, char *argv[]) {
    // only argument should be the input file name
    if (argc != 2) {
        printf("usage: raytracer1a <name of input file>\n");
        return 0;
    }

    std::ifstream input;

    input.open(argv[1]);
    if (!input.is_open()) {
        std::cout << "unable to open file." << std::endl;
        return 0;
    }

    //Vector3 eye;
    Vector3 viewdir;
    Vector3 updir;
    float hfov;
    int resolution[2] = { -1, -1 };
    Color bkgcolor;
    std::vector<Color> materials;
    std::vector<Sphere> objects;

    int read_inputs = 0;

    try {
        std::string line;
        int index = -1;
        while(std::getline(input, line)) {
            std::vector<std::string> keys = split(line, ' ');
            if (keys[0] == "eye") {
                Vector3 eye = Vector3(stof(keys[1]), stof(keys[2]), stof(keys[3]));
                read_inputs++;
                std::cout << "eye: " << eye.x() << " " << eye.y() << " " << eye.z() << std::endl;
            }
            else if (keys[0] == "viewdir") {
                viewdir.x = stof(keys[1]);
                viewdir.y = stof(keys[2]);
                viewdir.z = stof(keys[3]);
                read_inputs++;
                std::cout << "viewdir: " << viewdir.x << " " << viewdir.y << " " << viewdir.z << std::endl;
            }
            else if (keys[0] == "updir") {
                updir.x = stof(keys[1]);
                updir.y = stof(keys[2]);
                updir.z = stof(keys[3]);
                read_inputs++;
                std::cout << "updir: " << updir.x << " " << updir.y << " " << updir.z << std::endl;
            }
            else if (keys[0] == "hfov") {
                hfov = stof(keys[1]);
                read_inputs++;
                std::cout << "hfov: " << hfov << std::endl;
            }
            else if (keys[0] == "imsize") {
                resolution[0] = stoi(keys[1]);
                resolution[1] = stoi(keys[2]);
                read_inputs++;
                std::cout << "res: " << resolution[0] << " " << resolution[1] << std::endl;
            }
            else if (keys[0] == "bkgcolor") {
                std::cout << "r: " << keys[1] << std::endl;
                bkgcolor.r = stof(keys[1]);
                bkgcolor.g = stof(keys[2]);
                bkgcolor.b = stof(keys[3]);
                read_inputs++;
                std::cout << "bkg: " << bkgcolor.r << " " << bkgcolor.g << " " << bkgcolor.b << std::endl;
            }
            else if (keys[0] == "mtlcolor") {
                Color temp = {
                    .r = stoi(keys[1]),
                    .g = stoi(keys[2]),
                    .b = stoi(keys[3]),
                };
                std::cout << "mtlcolor: " << temp.r << " " << temp.g << " " << temp.b << std::endl; 
                materials.push_back(temp);
                index++;
            }
            else if (keys[0] == "sphere") {
                Sphere temp = {
                    .x = stof(keys[1]),
                    .y = stof(keys[2]),
                    .z = stof(keys[3]),
                    .r = stof(keys[4]),
                    .m = index
                };
                std::cout << "sphere: " << temp.x << " " << temp.y << " " << temp.z << " " << temp.r << std::endl; 
                objects.push_back(temp);
            }
            else {
                std::cout << "invalid key" << std::endl;
                return 0;
            }
        }

        // read fewer than 6 inputs
        if (read_inputs < 6) {
            std::cout << "missing a required input: [eye, viewdir, updir, hfov, res, bkgcolor]" << std::endl;
            return 0;
        }
        else if (index == -1 || objects.size() < 1) {
            std::cout << "missing at least one mtlcolor, or one sphere" << std::endl;
            return 0;
        }
    }
    catch (std::exception& e) {
        std::cout << "invalid arg" << std::endl;
        return 0;
    }

    int size = resolution[0] * resolution[1];
    Color *pixelmap = new Color[size];
    // define the 4 corners of the viewing window
    Vector3 ul;
    Vector3 ur;
    Vector3 ll;
    Vector3 lr;
    Vector3 w;



    delete(pixelmap);
    return 1;
}