#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <sstream>

#include "vector.h"
#include "ray.h"
#include "sphere.h"
#include "material.h"
#include "color.h"
#include "light.h"
#include "scene.h"


const float epsilon = 0.001;
const float pi = 4.0 * atan(1.0);

Scene scene;

// convert from a Color struct to a string
std::string pixel_to_string(Color pixel) {
    std::stringstream out;
    pixel.clamp();
    out << std::to_string(static_cast<int>(ceil(pixel.r() * 255)))
        << " "
        << std::to_string(static_cast<int>(ceil(pixel.g() * 255)))
        << " "
        << std::to_string(static_cast<int>(ceil(pixel.b() * 255))) << "\n";
    return out.str();
}

Color shade_ray(int s, Vector x_p, Vector view_v) {
    Sphere object = scene.get_object(s);
    Material mat = scene.get_material(object.material());
    Vector n = x_p - object.center();
    n = 1 / object.radius() * n;

    Color final_color = mat.ka() * mat.diffuse();

    for (int i = 0; i < scene.num_lights(); i++) {
        Light cur_light = scene.get_light(i);
        float s_flag = 1.0;
        // calculate the direction of the light source based on whether its a point or directional light
        Vector l = (cur_light.w()) ? cur_light.l() - x_p : -cur_light.l();
        float d = x_p.distance(cur_light.l());

        l.normalize();
        Ray r = Ray(x_p, l);

        // check if the light source is blocked by another object
        for (int j = 0; j < scene.num_objects(); j++) {
            // dont check yourself 
            if (j == s) { continue; }
            float t = r.intersect_sphere(scene.get_object(j));
            // is the intersection between the light source
            bool is_between = (cur_light.w()) ? epsilon < t && t < d : t > epsilon;
            if (is_between) {
                s_flag = 0.0;
                break;
            }
        }
        // no need to calculate diffuse or specular for this light source
        if (s_flag == 0.0) { continue; }
        float ndotl = std::max(0.0f, n.dot(l));
        // if n dot l is 0 then n dot h is also 0 so same situation
        if (ndotl == 0.0) { continue; }
        Vector h = l + view_v;
        h.normalize();
        float ndoth = std::max(0.0f, n.dot(h));
        Color diffuse = ndotl * mat.kd() * mat.diffuse();
        Color specular = std::pow(ndoth, mat.n()) * mat.ks() * mat.specular();
        final_color = final_color + (cur_light.intensity() * cur_light.atten(d)) * (diffuse + specular);
    }
    final_color = scene.depth_cue(x_p, final_color, scene.eye().distance(x_p));
    return final_color;
}



// given a ray returns the color of any intersected geometry
Color trace_ray(Ray ray) {
    float min_t = INFINITY;
    Color output = scene.background();
    int hit_sphere = -1;
    for (int i = 0; i < scene.num_objects(); i++) {
        float t = ray.intersect_sphere(scene.get_object(i));
        if (t > 0){
            if (t < min_t) {
                min_t = t;
                hit_sphere = i;
            }
        }
    }
    if (hit_sphere != -1){
        Vector x_p = ray.get_point(min_t);
        Vector view_v = ray.origin() - x_p;
        view_v.normalize();
        output = shade_ray(hit_sphere, x_p, view_v);
    }
    return output;
}


int main(int argc, char *argv[]) {
    // only argument should be the input file name
    if (argc != 2) {
        printf("usage: raytracer1a <name of input file>\n");
        return 0;
    }
    scene = Scene();
    if (scene.load_from_file(argv[1]) == 0) {
        return 0;
    }

    // scene parameters
    
    int size = scene.px_width() * scene.px_height();
    Color *pixelmap = new Color[size];

    // viewdir cross updir approaching invalidity with fp error
    if (scene.view().dot(scene.up()) < -0.9 || scene.view().dot(scene.up()) > 0.9) {
        std::cout << "up vector is too close to view vector" << std::endl;
        return 0;
    }

    // define the viewing coordinate system
    Vector u = scene.view().cross(scene.up());
    u.normalize();
    Vector v = u.cross(scene.view());

    float aspect = (float)scene.px_width() / (float)scene.px_height();
    float d = 1;

    // width is just frustum width for a parallel projection
    float width = (scene.is_parallel()) ? scene.frustum_width() : 2 * d * tan(scene.fov() / 2);
    float height = width / aspect;

    // go to view plane then to the left/right edge, then to the top/bottom
    Vector ul = (scene.eye() + d * scene.view()) - ((width / 2) * u) + ((height/2) * v);
    Vector ur = (scene.eye() + d *  scene.view()) + ((width / 2) * u) + ((height/2) * v);

    Vector ll = (scene.eye() + d *  scene.view()) - ((width / 2) * u) - ((height/2) * v);
    Vector lr = (scene.eye() + d *  scene.view()) + ((width / 2) * u) - ((height/2) * v);

    Vector deltah = (1.0 / (scene.px_width() - 1)) * (ur - ul);
    Vector deltav = (1.0 / (scene.px_height() - 1)) * (ll - ul);

    // origin is ul if parallel
    Vector origin = (scene.is_parallel()) ? ul : scene.eye();
    // direction is always viewdir if parallel
    Vector direction = (scene.is_parallel()) ? scene.view() : ul - scene.eye();

    Ray ray = Ray(origin, direction);

    // for every pixel in the output image trace a ray to get its color
    for (int i = 0; i < scene.px_height(); i++) {
        // for parallel the direction is always the same
        if (scene.is_parallel()) {
            ray.set_origin((ul + (i * deltav)));
        }
        // for perspective origin is always the eye
        else {
            ray.set_direction((ul + i * deltav) - scene.eye());
        }
        for (int j = 0; j < scene.px_width(); j++){
            int pos = j + (scene.px_width() * i);
            pixelmap[pos] = trace_ray(ray);
            
            if (scene.is_parallel()) {
                ray.set_origin((ul + (i * deltav) + (j * deltah)));
            }
            else {
                ray.set_direction((ul + (i * deltav) + (j * deltah)) - scene.eye());
            }
        }
    }

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
        << scene.px_width()
        << " "
        << scene.px_height()
        << "\n255\n";

    std::stringstream image;
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