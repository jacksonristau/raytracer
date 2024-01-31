#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <sstream>

#include "math/vector.h"
#include "math/ray.h"
#include "math/point.h"
#include "math/sphere.h"

typedef struct {
    int r, g, b;
} Color;

std::vector<Color> materials;
std::vector<Sphere> objects;

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

std::string pixel_to_string(Color pixel) {
    std::stringstream out;
    out << std::to_string(pixel.r)
        << " "
        << std::to_string(pixel.g)
        << " "
        << std::to_string(pixel.b) << "\n";
    return out.str();
}

Color shade_ray(int m) {
    return materials[m];
}

Color trace_ray(Ray ray, Color bkgcolor) {
    float min_t = INFINITY;
    float t;
    Color output = bkgcolor;
    for (int i = 0; i < objects.size(); i++) {
        float t = ray.intersect_sphere(objects[i]);
        if (t > 0){
            if (t < min_t) {
                min_t = t;
                output = shade_ray(objects[i].material());
            }
        }
    }
    return output;
}

int main(int argc, char *argv[]) {
    // only argument should be the input file name
    if (argc != 2) {
        printf("usage: raytracer1a <name of input file>\n");
        return 0;
    }

    std::ifstream input;
    std::ofstream output;

    input.open(argv[1]);
    if (!input.is_open()) {
        std::cout << "unable to open file." << std::endl;
        return 0;
    }

    Point eye;
    Vector viewdir;
    Vector updir;
    float hfov;
    int resolution[2] = { -1, -1 };
    Color bkgcolor;
    float pi = 4.0 * atan(1.0);

    int read_inputs = 0;

    // read values from file
    try {
        std::string line;
        int index = -1;
        while(std::getline(input, line)) {
            std::vector<std::string> keys = split(line, ' ');
            if (keys[0] == "eye") {
                if (keys.size() != 4) {
                    std::cout << "3 numbers are required for eye" << std::endl;
                    return 0;
                }
                eye = Point(stof(keys[1]), stof(keys[2]), stof(keys[3]));
                read_inputs++;
                std::cout << "eye: " << eye.x() << " " << eye.y() << " " << eye.z() << std::endl;
            }
            else if (keys[0] == "viewdir") {
                if (keys.size() != 4) {
                    std::cout << "3 numbers are required for viewdir" << std::endl;
                    return 0;
                }
                viewdir = Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]));
                read_inputs++;
                std::cout << "viewdir: " << viewdir.x() << " " << viewdir.y() << " " << viewdir.z() << std::endl;
            }
            else if (keys[0] == "updir") {
                if (keys.size() != 4) {
                    std::cout << "3 numbers are required for updir" << std::endl;
                    return 0;
                }
                updir = Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]));
                read_inputs++;
                std::cout << "updir: " << updir.x() << " " << updir.y() << " " << updir.z() << std::endl;
            }
            else if (keys[0] == "hfov") {
                if (keys.size() != 2) {
                    std::cout << "1 number is required for hfov" << std::endl;
                    return 0;
                }
                hfov = stof(keys[1]);
                // convert to radians
                hfov *= pi / 180;
                read_inputs++;
                std::cout << "hfov: " << hfov << std::endl;
            }
            else if (keys[0] == "imsize") {
                if (keys.size() != 3) {
                    std::cout << "2 numbers are required for imsize" << std::endl;
                    return 0;
                }
                resolution[0] = stoi(keys[1]);
                resolution[1] = stoi(keys[2]);
                read_inputs++;
                std::cout << "res: " << resolution[0] << " " << resolution[1] << std::endl;
            }
            else if (keys[0] == "bkgcolor") {
                if (keys.size() != 4) {
                    std::cout << "3 numbers are required for bkgcolor" << std::endl;
                    return 0;
                }
                bkgcolor.r = ceil(stof(keys[1]) * 255);
                bkgcolor.g = ceil(stof(keys[1]) * 255);
                bkgcolor.b = ceil(stof(keys[1]) * 255);
                read_inputs++;
                std::cout << "bkg: " << bkgcolor.r << " " << bkgcolor.g << " " << bkgcolor.b << std::endl;
            }
            else if (keys[0] == "mtlcolor") {
                if (keys.size() != 4) {
                    std::cout << "3 numbers are required for mtlcolor" << std::endl;
                    return 0;
                }
                Color temp = {
                    .r = ceil(stof(keys[1]) * 255),
                    .g = ceil(stof(keys[2]) * 255),
                    .b = ceil(stof(keys[3]) * 255),
                };
                std::cout << "mtlcolor: " << temp.r << " " << temp.g << " " << temp.b << std::endl; 
                materials.push_back(temp);
                index++;
            }
            else if (keys[0] == "sphere") {
                if (keys.size() != 5) {
                    std::cout << "4 numbers are required for spheres" << std::endl;
                    return 0;
                }
                Point center = Point(stof(keys[1]), stof(keys[2]), stof(keys[3]));
                float radius = stof(keys[4]);
                objects.push_back(Sphere(center, radius, index));
                std::cout << "sphere: " << center.x() << " " << center.y() << " " << center.z() << " " << radius << std::endl; 
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

        std::cout << "# of material colors: " << index + 1 << std::endl;
        std::cout << "# of objects: " << objects.size() << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "invalid arg" << std::endl;
        return 0;
    }

    int size = resolution[0] * resolution[1];
    Color *pixelmap = new Color[size];
    // define the 4 corners of the viewing window
    viewdir.normalize();
    Vector w = -viewdir;

    // viewdir cross updir == 0
    if (viewdir.dot(updir) < -0.9 || viewdir.dot(updir) > 0.9) {
        std::cout << "up vector is too close to view vector" << std::endl;
        return 0;
    } 
    Vector u = viewdir.cross(updir);
    u.normalize();
    Vector v = u.cross(viewdir);

    float aspect = (float)resolution[0] / (float)resolution[1];
    const float d = 10;

    float width = 2 * d * tan(hfov / 2);
    float height = width / aspect;

    // go to view plane then to the left/right edge, then to the top/bottom
    Point ul = (eye + d * viewdir) - ((width / 2) * u) + ((height/2) * v);
    Point ur = (eye + d *  viewdir) + ((width / 2) * u) + ((height/2) * v);

    Point ll = (eye + d *  viewdir) - ((width / 2) * u) - ((height/2) * v);
    Point lr = (eye + d *  viewdir) + ((width / 2) * u) - ((height/2) * v);

    Vector deltah = (1.0 / (resolution[0] - 1)) * (ur - ul) ;
    Vector deltav = (1.0 / (resolution[1] - 1)) * (ll - ul);

    Ray ray = Ray(eye, ul - eye);

    for (int i = 0; i < resolution[1]; i++) {
        if (i != 0){
            ray.set_direction(eye - (ul + i * deltav));
        }
        for (int j = 0; j < resolution[0]; j++){
            int pos = j + (resolution[0] * i);
            pixelmap[pos] = trace_ray(ray, bkgcolor);
            ray.set_direction((ul + (i * deltav) + (j * deltah)) - eye);
        }
    }

    std::string filename = argv[1];
    filename.std::string::erase(filename.std::string::find("."));
    output.open(filename + ".ppm");

    if (!output.is_open()) {
        std::cout << "failed to create output file.";
        return 0;
    }

    output << "P3\n"
        << "#fractal\n"
        << resolution[0]
        << " "
        << resolution[1]
        << "\n255\n";

    std::stringstream image;
    for (int i = 0; i < size; i++) {
        if (i % (size / 100) == 0) {
            std::cout << (i * 100) / size << '%' << '\r';
        }
        image << pixel_to_string(pixelmap[i]);
        if (i == size - 1) {
            std::cout << "100% done" << std::endl;
            std::cout << filename << ".ppm created." << std::endl;
        }
    }
    output << image.str();
    output.close();
    delete [] pixelmap;
    return 1;
}