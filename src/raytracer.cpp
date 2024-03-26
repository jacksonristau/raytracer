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


const float epsilon = 0.0001;
const float pi = 4.0 * atan(1.0);

Scene scene;
bool checklight = false;

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

Color trace_ray(Ray ray, bool entering, int depth = 0, std::vector<float> refract_stack = {scene.eta()});

float fresnel(float cosi, float ni, float nt, bool reflect) {
    float fo;
    if (reflect){
        fo = ((ni - 1) / (ni + 1));
    }
    else {
        fo = (nt - ni) / (nt + ni);
    }
    fo = fo * fo;
    return fo + (1 - fo) * std::pow((1 - cosi), 5);
}

// if bary is NULL then the intersection is with a sphere
Color shade_ray(int m, int o, Vector n, Vector x_p, Ray i_ray, int depth, bool entering, std::vector<float> refract_stack, float* bary) {
    Material mat = scene.get_material(m);
    Color d_lambda;
    float uv[2];
    //std::cout << "material: " << mat.get_texture() << std::endl;
    if (mat.has_texture()) {
        if (bary == NULL) {
            scene.get_sphere(o).get_uv(x_p, uv);
            //std::cout << "u: " << uv[0] << " v: " << uv[1] << std::endl;
        }
        else{
            std::vector<std::vector<float>> uvs = scene.get_uvs(o);
            uv[0] = (bary[0] * uvs[0][0]) + (bary[1] * uvs[1][0]) + (bary[2] * uvs[2][0]);
            uv[1] = (bary[0] * uvs[0][1]) + (bary[1] * uvs[1][1]) + (bary[2] * uvs[2][1]);
        }
        d_lambda = scene.get_texture_color(mat.get_texture(), uv[0], uv[1]);
    }
    // if theres no texture defined for the material the uvs dont matter
    else {
        d_lambda = mat.diffuse();
    }
    Color final_color = mat.ka() * d_lambda;
    if (depth >= 10){
        return final_color;
    }
    // if (bary != NULL) {
    //     std::cout << "fresnel: " << mat.fresnel() << std::endl;
    // }

    // for each light source in the scene calculate the diffuse and specular components
    for (int i = 0; i < scene.num_lights(); i++) {
        Light cur_light = scene.get_light(i);
        bool is_point = cur_light.w();
        float s_flag = 1.0;
        // calculate the direction of the light source based on whether its a point or directional light
        Vector l = (is_point) ? cur_light.l() - x_p : -cur_light.l();
        float d = x_p.distance(cur_light.l());

        l.normalize();
        Ray r = Ray(x_p, l);

        // check if the light source is blocked by any sphere
        for (int j = 0; j < scene.num_spheres(); j++) {
            // dont check yourself 
            if (j == o && bary == NULL) { continue; }
            float t = r.intersect_sphere(scene.get_sphere(j));
            float surface_alpha = scene.get_material(scene.get_sphere(j).material()).alpha();
            // is the intersection between the light source
            bool is_between = (is_point) ? epsilon < t && t < d : t > epsilon;
            if (is_between) {
                s_flag = s_flag * (1 - surface_alpha);
            }
        }
        // check if the light source is blocked by any triangle
        for (int j = 0; j < scene.num_indices(); j++) {
            // dont check yourself 
            if (j == o && bary != NULL) { continue; }
            float t = r.intersect_triangle(scene.get_vertices(j), NULL);
            float surface_alpha = scene.get_material(scene.get_material_index(j)).alpha();
            // is the intersection between the light source
            bool is_between = (is_point) ? epsilon < t && t < d : t > epsilon;
            if (is_between) {
                s_flag = s_flag * (1 - surface_alpha);
            }
        }
        Vector I = -i_ray.direction();
        // if (n.dot(I) < 0) {
        //     n = -n;
        // }
        // no need to calculate diffuse or specular for this light source
        float ndotl = std::max(0.0f, n.dot(l));
        // if n dot l is 0 then n dot h is also 0 so same situation
        if (ndotl == 0.0) { continue; }
        I.normalize();
        Vector h = l + I;
        h.normalize();
        float ndoth = std::max(0.0f, n.dot(h));
        Color diffuse = ndotl * mat.kd() * d_lambda;
        Color specular = std::pow(ndoth, mat.n()) * mat.ks() * mat.specular();
        Color reflection = Color(0, 0, 0);
        Color transparent = Color(0, 0, 0);
        if (mat.ks() > 0.0){
            float fr = fresnel(n.dot(I), mat.eta(), 0.0, true);
            reflection = fr * trace_ray(Ray(x_p, i_ray.reflect(n)), entering, depth + 1,  refract_stack);
        }
        if (mat.alpha() != 1.0){
            float ni = refract_stack.back();
            refract_stack.pop_back();
            float nt;
            if (entering) {
                nt = mat.eta();
            }
            // exiting object
            else {
                nt = scene.eta();
            }
            refract_stack.push_back(nt);
            Vector refract_dir = i_ray.refract(n, ni, nt);
            float fr = fresnel(n.dot(I), ni, nt, false);
            if (fr > 1.0) {
                std::cout << "fr: " << fr << std::endl;
            }
            transparent = (1 - fr) * (1 - mat.alpha()) * trace_ray(Ray(x_p, refract_dir), !entering, depth + 1, refract_stack);
            if (transparent.r() < 0 || transparent.g() < 0 || transparent.b() < 0) {
                std::cout << ", fr: " << fr;
                std::cout << ", ni: " << ni;
                std::cout << ", nt: " << nt;
                std::cout << ", n dot i: " << n.dot(I);
                std::cout << ", N: " << n;
                std::cout << ", I: " << I;
                std::cout << ", transparent: " << transparent << std::endl;
            }
        }

        final_color = final_color + (s_flag * ((cur_light.intensity() * cur_light.atten(d)) * (diffuse + specular)) + reflection + transparent);
        if (final_color.r() < 0.05 || final_color.g() < 0.05 || final_color.b() < 0.05) {
            std::cout << "n: " << n;
            std::cout << ", l: " << l;
            std::cout << ", n dot l: " << ndotl;
            std::cout << ", depth: " << depth;
            std::cout << ", diffuse: " << diffuse;
            std::cout << ", specular: " << specular;
            std::cout << ", reflection: " << reflection;
            std::cout << ", transparent: " << transparent << std::endl;
        }
    }
    final_color = scene.depth_cue(x_p, final_color, scene.eye().distance(x_p));
    return final_color;
}



// given a ray returns the color of any intersected geometry
Color trace_ray(Ray ray, bool entering, int depth, std::vector<float> refract_stack) {
    float min_t = INFINITY;
    Color output = scene.background();
    int hit_index = -1;
    bool is_sphere = false;
    // each sphere in scene
    for (int i = 0; i < scene.num_spheres(); i++) {
        float t = ray.intersect_sphere(scene.get_sphere(i));
        if (t > epsilon){
            if (t < min_t) {
                min_t = t;
                hit_index = i;
                is_sphere = true;
            }
        }
    }
    float bary[3];
    for (int i = 0; i < scene.num_indices(); i++) {
        // bary is filled with the barycentric coordinates of the intersection
        float t = ray.intersect_triangle(scene.get_vertices(i), bary);
        if (t > epsilon){
            if (t < min_t) {
                min_t = t;
                hit_index = i;
                is_sphere = false;
            }
        }
    }

    // if the ray intersects an object
    if (hit_index != -1){
        Vector intersection = ray.get_point(min_t);
        int m_index;
        Vector n;
        // if the ray intersects a sphere
        if (is_sphere) {
            if (entering == false) {
                n = scene.get_sphere(hit_index).center() - intersection;
            }
            else {
                n = intersection - scene.get_sphere(hit_index).center();
            }
            m_index = scene.get_sphere(hit_index).material();
            n = (1 / scene.get_sphere(hit_index).radius()) * n;
        }
        // if the ray intersects a triangle
        else {
            m_index = scene.get_material_index(hit_index);
            std::vector<Vector> normals = scene.get_normals(hit_index);
            // if no normals are defined 
            if (normals.size() == 0){
                std::vector<Vector> vertices = scene.get_vertices(hit_index);
                n = (vertices[1] - vertices[0]).cross(vertices[2] - vertices[0]);
                n.normalize();
            }
            // use the normals defined and interpolate
            else {
                n = bary[0] * normals[0] + bary[1] * normals[1] + bary[2] * normals[2];
                n.normalize();
            }
        }
        output = shade_ray(m_index, hit_index, n, intersection, ray, depth, entering, refract_stack, (is_sphere) ? NULL : bary);
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
    std::cout << "v: " << v << std::endl;

    float aspect = (float)scene.px_width() / (float)scene.px_height();
    float d = 2;

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
    std::cout << "tracing rays..." << std::endl;
    std::cout << "0% complete...";
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
            try{
                pixelmap[pos] = trace_ray(ray, true);
            }
            catch (std::exception& e){
                std::cout << e.what() << std::endl;
                return 0;
            }
            
            // if (j % 10 == 0) {
            //     std::cout << '\r' << (int)((float)pos / (float)size * 100) << "% complete..." << std::flush;
            // }
            if (scene.is_parallel()) {
                ray.set_origin((ul + (i * deltav) + (j * deltah)));
            }
            else {
                ray.set_direction((ul + (i * deltav) + (j * deltah)) - scene.eye());
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
        << scene.px_width()
        << " "
        << scene.px_height()
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