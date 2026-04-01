#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <thread>
#include <atomic>
#include <filesystem>

#include "../include/math/vector3.h"
#include "../include/math/ray.h"
#include "../include/gfx/color.h"
#include "../include/gfx/scene.h"
#include "../include/gfx/camera.h"
#include "../include/gfx/raytracer.h"
#include "../include/stringhelpers.h"


using json = nlohmann::json;
bool debug_mode_enabled = false;

Color* render_image_serial(int size) {
    Color* pixelmap = new Color[size];
    for (int i = 0; i < Scene::camera.px_height(); i++) {
        for (int j = 0; j < Scene::camera.px_width(); j++) {
            if (i == 167 && j == 115) {
                std::cout << "good";
            }
            if (i == 289 && j == 103) {
                std::cout << "bad";
            }
            int pos = j + (Scene::camera.px_width() * i);
            Ray ray = Scene::camera.generate_ray(j, i);
            Color pixel_color = Raytracer::trace_debug(ray, 1);
            pixelmap[pos] = pixel_color;
        }
    }
    return pixelmap;
}

void render_row(int row, Color* pixelmap) {
    for (int i = 0; i < Scene::camera.px_width(); i++) {
        int pos = i + (Scene::camera.px_width() * row);
        Ray ray = Scene::camera.generate_ray(i, row);
        try {
            Color pixel_color = Raytracer::trace_ray(ray, true);
            pixelmap[pos] = pixel_color;
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            return;
        }
        catch (const std::string* str) {
            std::cout << "failed to trace ray: " << i << ", " << row << '\n' << str << '\n';
            return;
        }
    }
}
void render_row_aa(int row, Color* pixelmap) {
    for (int i = 0; i < Scene::camera.px_width(); i++) {
        int pos = i + (Scene::camera.px_width() * row);
        std::vector<Ray> rays = Scene::camera.generate_ray_aa(i, row);
        try {
            Color pixel_color = Color(0.0f, 0.0f, 0.0f);
            for (Ray ray : rays) {
                pixel_color = pixel_color + Raytracer::trace_ray(ray, true);
            }
            pixel_color = pixel_color / (Scene::camera.n_samples * Scene::camera.n_samples);

            pixelmap[pos] = pixel_color;
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            return;
        }
        catch (const std::string* str) {
            std::cout << "failed to trace ray: " << i << ", " << row << '\n' << str << '\n';
            return;
        }
    }
}

void render_rows_static(int start, int end, Color* pixelmap) {
    for (int i = start; i < end; i++) {
        render_row(i, pixelmap);
    }
}

void render_rows_dynamic(std::atomic<int>& next_row, Color* pixelmap) {
    int my_row;
    int height = Scene::camera.px_height();

    while ((my_row = next_row.fetch_add(1)) < height) {
        if (Scene::camera.n_samples < 2)
            render_row(my_row, pixelmap);
        else
            render_row_aa(my_row, pixelmap);
    }
}

Color* multi_render_static(int num_threads, int img_size) {
    Color* pixelmap = new Color[img_size];
    std::vector<std::thread> threads(num_threads);
    int rows_per_thread = Scene::camera.px_height() / num_threads;

    std::cout << "rendering..." << '\n';
    // block rows to render for each thread
    for (int i = 0; i < num_threads; i++) {
        int start_row = i * rows_per_thread;
        int end_row = (i == num_threads - 1) ? Scene::camera.px_height() : start_row + rows_per_thread;
        threads[i] = std::thread(render_rows_static, start_row, end_row, pixelmap);
    }
    for (auto& t : threads) t.join();
    return pixelmap;
}

Color* multi_render_dynamic(int num_threads, int img_size) {
    Color* pixelmap = new Color[img_size];
    std::vector<std::thread> threads(num_threads);
    std::atomic<int> next_row = 0;

    std::cout << "rendering..." << '\n';
    for (int i = 0; i < num_threads; i++) {
        threads[i] = std::thread(render_rows_dynamic, std::ref(next_row), pixelmap);
    }
    for (auto& t : threads) t.join();
    return pixelmap;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: raytracer-cli <name of input file> [options]\n");
        printf("options:\n");
        printf("  -W, --wireframe       enable wireframe rendering (bool)\n");
        printf("  -D, --depth           set recursion max depth (int)\n");
        printf("  -E, --edge-threshold  set edge detection threshold (float)\n");
        printf("  -S, --samples         set number of sub pixel samples (int)");
        printf("  -d, --debug           enable debug ray tracing");
        return 0;
    }
    // Parse command-line arguments
    std::string scene_file;
    int n_samples = 2;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-W" || arg == "--wireframe") {
            Raytracer::wireframe_mode = true;
            std::cout << "wireframe mode enabled" << std::endl;
        } 
        else if (arg == "-d" || arg == "--debug") {
            debug_mode_enabled= true;
            std::cout << "debug mode enabled" << std::endl;
        } 
        else if ((arg == "-D" || arg == "--depth") && i+1 < argc) {
            try{
                const std::string val = argv[++i];
                int depth = std::stoi(val);
                Raytracer::max_depth = std::stoi(val);
                std::cout << "max depth: " << val << "\n";
            }
            catch (std::invalid_argument const& ex)
            {
                std::cout << "invalid depth argument " << ex.what() << '\n';
                return 1;
            }
        }
        else if ((arg == "-S" || arg == "--samples") && i+1 < argc) {
            try{
                const std::string val = argv[++i];
                int depth = std::stoi(val);
                n_samples = std::stoi(val);
                std::cout << "samples: " << val << "\n";
            }
            catch (std::invalid_argument const& ex)
            {
                std::cout << "invalid depth argument " << ex.what() << '\n';
                return 1;
            }
        }
        else if ((arg == "-E" || arg == "--edge-threshold") && i+1 < argc) {
            try{
                const std::string val = argv[++i];
                float et = std::stof(val);
                Raytracer::edge_threshold = std::stof(val);
                std::cout << "edge threshold: " << val << "\n";
            }
            catch (std::invalid_argument const& ex)
            {
                std::cout << "invalid edge-threshold argument " << ex.what() << '\n';
                return 1;
            }
        }
        else if (arg[0] != '-') {
            // Non-flag argument is the scene file
            if (scene_file.empty()) {
                scene_file = arg;
            } else {
                std::cerr << "error: multiple input files specified\n";
                return 1;
            }
        } 
        else {
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
        std::cerr << "cwd " << std::filesystem::current_path() << '\n';
        return 1;
	}
    Scene::camera.n_samples = n_samples;
    
    Raytracer::bvh = BVH(Scene::primitives);
    uint32_t next_node = 0;
    Raytracer::bvh.build_bvh_object_median(next_node, 0, Scene::primitives.size());

	int hardware_threads = std::thread::hardware_concurrency();
	int num_threads = hardware_threads > 0 ? hardware_threads - 1 : 2;
    std::cout << "utilizing " << num_threads << " threads" << '\n';

    int size = Scene::camera.px_width() * Scene::camera.px_height();
    Color* pixelmap;
    if (debug_mode_enabled)
        pixelmap = render_image_serial(size);
    else
        pixelmap = multi_render_dynamic(num_threads, size);
    
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
    return 0;
}