#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <nlohmann/json.hpp>

#include "../include/math/vector3.h"
#include "../include/math/ray.h"
#include "../include/gfx/color.h"
#include "../include/gfx/scene.h"
#include "../include/gfx/camera.h"
#include "../include/gfx/raytracer.h"
#include "../include/stringhelpers.h"


using json = nlohmann::json;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: raytracer-cli <name of input file> [options]\n");
        printf("options:\n");
        printf("  -W, --wireframe     Enable wireframe rendering\n");
        return 0;
    }

    // Parse command-line arguments
    std::string scene_file;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-W" || arg == "--wireframe") {
            Raytracer::wireframe_mode = true;
            std::cout << "Wireframe mode enabled" << std::endl;
        } else if (arg[0] != '-') {
            // Non-flag argument is the scene file
            if (scene_file.empty()) {
                scene_file = arg;
            } else {
                std::cerr << "error: multiple input files specified\n";
                return 1;
            }
        } else {
            std::cerr << "error: unknown option '" << arg << "'\n";
            return 1;
        }
    }

    if (scene_file.empty()) {
        std::cerr << "error: no input file specified\n";
        return 1;
    }

    auto f = std::ifstream(scene_file);
    if (!f) {
        std::cerr << "failed to open: " << scene_file << '\n';
        return 1;
    }

    Scene scene;
    try {
        json scene_description = json::parse(f);
        scene = Scene(scene_description);
    }
	catch(std::exception& e) {
        std::cerr << "failed to parse scene: " << e.what() << '\n';
        return 1;
	}
    
    int size = Scene::camera.px_width() * Scene::camera.px_height();
    Color *pixelmap = new Color[size];

    // for every pixel in the output image trace a ray to get its color
    std::cout << "tracing rays..." << std::endl;
    std::cout << "0% complete...";
    for (int i = 0; i < Scene::camera.px_height(); i++) {
        for (int j = 0; j < Scene::camera.px_width(); j++){
            int pos = j + (Scene::camera.px_width() * i);
            Ray ray = Scene::camera.generate_ray(j, i);
            try{
                Color pixel_color = Raytracer::trace_ray(ray, true);
                if (pos < size)
                    pixelmap[pos] = pixel_color;
            }
            catch (std::exception& e){
                std::cout << e.what() << std::endl;
                return 0;
            }
            catch(const std::string* str){
                std::cout << "failed to trace ray: " << j << ", " << i << '\n' << str << '\n';
                return 0;
            }
            if (j % 100 == 0) {
                std::cout << '\r' << (int)((float)pos / (float)size * 100) << "% complete..." << std::flush;
                /*std::cout << "ray_dir: " << ray.direction() << '\n';
                std::cout << "i: " << i << "j: " << j << '\n';*/
            }
        }
    }
    std::cout << '\r';
    std::cout << "tracing complete." << std::endl;

    std::ofstream output;
    output.open("render.ppm");

    if (!output.is_open()) {
        std::cout << "failed to create output file.";
        return 0;
    }

    // write colors to ppm file 
    output << "P3\n"
        << "#raytracingiscool\n"
        << Scene::camera.px_width()
        << " "
        << Scene::camera.px_height()
        << "\n255\n";

    std::stringstream image;
    std::cout << "creating image..." << std::endl;
    for (int i = 0; i < size; i++) {
        image << pixel_to_string(pixelmap[i]);
        if (i == size - 1) {
            std::cout << "render.ppm created." << std::endl;
        }
    }
    output << image.str();

    // cleanup
    output.close();
    delete [] pixelmap;
    return 1;
}