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
    // only argument should be the input file name
    if (argc != 2) {
        printf("usage: raytracer-cli <name of input file>\n");
        return 0;
    }
    auto f = std::ifstream(argv[1]);
    if (!f) {
        std::cerr << "failed to open: " << argv[1] << '\n';
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
            
            if (j % 100 == 0) {
                std::cout << '\r' << (int)((float)pos / (float)size * 100) << "% complete..." << std::flush;
                /*std::cout << "ray_dir: " << ray.direction() << '\n';
                std::cout << "i: " << i << "j: " << j << '\n';*/
            }
        }
    }
    std::cout << '\r';
    std::cout << "tracing complete." << std::endl;

    std::string filename = argv[1];
    filename.std::string::erase(filename.std::string::find("."));
    std::ofstream output;
    output.open(filename + ".ppm");

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
            std::cout << filename << ".ppm created." << std::endl;
        }
    }
    output << image.str();

    // cleanup
    output.close();
    delete [] pixelmap;
    return 1;
}