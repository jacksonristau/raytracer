#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <nlohmann/json.hpp>
#include <thread>
#include <atomic>
#include <filesystem>

#include "../include/math/ray.h"
#include "../include/gfx/color.h"
#include "../include/gfx/scene.h"
#include "../include/gfx/camera.h"
#include "../include/gfx/raytracer.h"
#include "../include/stringhelpers.h"

using json = nlohmann::json;

// traces every pixel in a row, averaging n_samples x n_samples jittered rays per pixel when anti-aliasing
void render_row(int row, Color* pixelmap) {
    const int n_samples = Scene::camera.n_samples;
    for (int i = 0; i < Scene::camera.px_width(); i++) {
        int pos = i + (Scene::camera.px_width() * row);
        try {
            if (n_samples < 2) {
                pixelmap[pos] = Raytracer::trace_ray(Scene::camera.generate_ray(i, row), 1);
                continue;
            }
            Color pixel_color = Color(0.0f, 0.0f, 0.0f);
            for (const Ray& ray : Scene::camera.generate_ray_aa_urandom(i, row)) {
                pixel_color = pixel_color + Raytracer::trace_ray(ray, 1);
            }
            pixelmap[pos] = pixel_color / static_cast<float>(n_samples * n_samples);
        }
        catch (const std::exception& e) {
            std::cerr << "failed to trace pixel (" << i << ", " << row << "): " << e.what() << '\n';
            return;
        }
    }
}

// single-threaded, top to bottom; deterministic ordering makes it easier to step through in a debugger
Color* render_serial(int img_size) {
    Color* pixelmap = new Color[img_size];
    for (int row = 0; row < Scene::camera.px_height(); row++) {
        render_row(row, pixelmap);
    }
    return pixelmap;
}

// each thread grabs the next unrendered row, so fast and slow regions of the image balance out
void render_rows_dynamic(std::atomic<int>& next_row, Color* pixelmap) {
    int my_row;
    int height = Scene::camera.px_height();

    while ((my_row = next_row.fetch_add(1)) < height) {
        render_row(my_row, pixelmap);
    }
}

Color* render_parallel(int num_threads, int img_size) {
    Color* pixelmap = new Color[img_size];
    std::vector<std::thread> threads(num_threads);
    std::atomic<int> next_row = 0;

    for (int i = 0; i < num_threads; i++) {
        threads[i] = std::thread(render_rows_dynamic, std::ref(next_row), pixelmap);
    }
    for (auto& t : threads) t.join();
    return pixelmap;
}

void print_usage() {
    std::cout << "usage: raytracer-cli <scene.json> [options]\n"
              << "options:\n"
              << "  -S, --samples <n>         anti-aliasing: n x n jittered samples per pixel (default 2, 1 disables)\n"
              << "  -D, --depth <n>           max reflection/refraction recursion depth (default 4)\n"
              << "  -W, --wireframe           render triangle edges only\n"
              << "  -E, --edge-threshold <f>  barycentric threshold for wireframe edges (default 0.02)\n"
              << "  -d, --debug               render on a single thread\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string scene_file;
    int n_samples = 2;
    bool single_threaded = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        bool has_value = i + 1 < argc;
        try {
            if (arg == "-h" || arg == "--help") {
                print_usage();
                return 0;
            }
            else if (arg == "-W" || arg == "--wireframe") {
                Raytracer::wireframe_mode = true;
            }
            else if (arg == "-d" || arg == "--debug") {
                single_threaded = true;
            }
            else if ((arg == "-D" || arg == "--depth") && has_value) {
                Raytracer::max_depth = std::stoi(argv[++i]);
            }
            else if ((arg == "-S" || arg == "--samples") && has_value) {
                n_samples = std::stoi(argv[++i]);
            }
            else if ((arg == "-E" || arg == "--edge-threshold") && has_value) {
                Raytracer::edge_threshold = std::stof(argv[++i]);
            }
            else if (arg[0] != '-') {
                if (!scene_file.empty()) {
                    std::cerr << "error: multiple input files specified\n";
                    return 1;
                }
                scene_file = arg;
            }
            else {
                std::cerr << "error: unknown option or missing value: " << arg << "\n";
                return 1;
            }
        }
        catch (const std::logic_error&) {
            std::cerr << "error: invalid value for " << arg << ": " << argv[i] << "\n";
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
    catch (const std::exception& e) {
        std::cerr << "failed to load scene: " << e.what() << '\n';
        std::cerr << "relative paths are resolved from " << std::filesystem::current_path() << '\n';
        return 1;
    }
    Scene::camera.n_samples = n_samples;

    Raytracer::bvh = BVH(Scene::primitives);
    uint32_t next_node = 0;
    Raytracer::bvh.build_bvh_object_median(next_node, 0, Scene::primitives.size());

    int size = Scene::camera.px_width() * Scene::camera.px_height();
    Color* pixelmap;
    if (single_threaded) {
        std::cout << "rendering on 1 thread..." << '\n';
        pixelmap = render_serial(size);
    }
    else {
        int hardware_threads = std::thread::hardware_concurrency();
        int num_threads = hardware_threads > 1 ? hardware_threads - 1 : 1;
        std::cout << "rendering on " << num_threads << " threads..." << '\n';
        pixelmap = render_parallel(num_threads, size);
    }

    std::ofstream output("render.ppm");
    if (!output.is_open()) {
        std::cerr << "failed to create render.ppm\n";
        delete [] pixelmap;
        return 1;
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
    }
    output << image.str();
    output.close();
    delete [] pixelmap;

    std::cout << "render.ppm created." << std::endl;
    return 0;
}
