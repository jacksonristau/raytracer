#include <iostream>
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

std::vector<Material> materials;
std::vector<Sphere> objects;
std::vector<Light> lights;

// depth cueing
Color dc = Color(-1, -1, -1);
float depth_alpha[2];
float depth_dist[2];

Color bkgcolor;
float epsilon = 0.01;

// tokenizes a list by the delimiter
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
    Material mat = materials[objects[s].material()];
    Vector n = x_p - objects[s].center();
    n = 1 / objects[s].radius() * n;

    Color final_color = mat.ka() * mat.diffuse();

    for (int i = 0; i < lights.size(); i++) {
        float s_flag = 1.0;
        // calculate the direction of the light source based on whether its a point or directional light
        Vector l = (lights[i].w()) ? lights[i].l() - x_p : -lights[i].l();
        float d = x_p.distance(lights[i].l());

        l.normalize();
        Ray r = Ray(x_p, l);

        // check if the light source is blocked by another object
        for (int j = 0; j < objects.size(); j++) {
            // dont check yourself 
            if (j == s) { continue; }
            float t = r.intersect_sphere(objects[j]);
            // is the intersection between the light source
            bool is_between = (lights[i].w()) ? epsilon < t && t < d : t > epsilon;
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
        final_color = final_color + (lights[i].intensity() * lights[i].atten(d)) * (diffuse + specular);
        if (dc != Color(-1, -1, -1)) {
            float alpha;
            if (d <= depth_dist[0]) {
                alpha = depth_alpha[1];
            }
            else if (d >= depth_dist[1]) {
                alpha = depth_alpha[0];
            }
            else {
                alpha = depth_alpha[0] + (depth_alpha[1] - depth_alpha[0]) * ((depth_dist[1] - d) / (depth_dist[1] - depth_dist[0]));
            }
            final_color = alpha * final_color + (1 - alpha) * dc;
        }
    }
    return final_color;
}



// given a ray returns the color of any intersected geometry
Color trace_ray(Ray ray) {
    float min_t = INFINITY;
    Color output = bkgcolor;
    int hit_sphere = -1;
    for (int i = 0; i < objects.size(); i++) {
        float t = ray.intersect_sphere(objects[i]);
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

    std::ifstream input;
    std::ofstream output;

    input.open(argv[1]);
    if (!input.is_open()) {
        std::cout << "unable to open file." << std::endl;
        return 0;
    }

    // scene parameters
    Vector eye;
    Vector viewdir;
    Vector updir;
    float hfov;
    int resolution[2] = { -1, -1 };
    float frustum_w = -1.0;
    bool parallel = false;
    float pi = 4.0 * atan(1.0);

    // depth cueing
    Color dc = Color(-1, -1, -1);
    float depth_alpha[2];
    float depth_dist[2];

    // if theres fewer somethings wrong
    int read_inputs = 0;

    // read values from file
    try {
        std::string line;
        int index = -1;
        while(std::getline(input, line)) {
            if (line.empty()) {
                continue;
            }
            std::vector<std::string> keys = split(line, ' ');
            if (keys[0] == "eye") {
                if (keys.size() != 4) {
                    std::cout << "3 numbers are required for eye" << std::endl;
                    return 0;
                }
                eye = Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]), 1.0);
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
                bkgcolor = Color(stof(keys[1]), stof(keys[2]), stof(keys[3]));
                read_inputs++;
                std::cout << "bkg: " << bkgcolor << std::endl;
            }
            else if (keys[0] == "mtlcolor") {
                if (keys.size() != 11) {
                    std::cout << "10 numbers are required for mtlcolor" << std::endl;
                    return 0;
                }
                Color d = Color(stof(keys[1]), stof(keys[2]), stof(keys[3]));
                Color s = Color(stof(keys[4]), stof(keys[5]), stof(keys[6]));
                // if a specular isnt given use the other constructor
                materials.push_back(Material(d, s, stof(keys[7]), stof(keys[8]), stof(keys[9]), stof(keys[10])));
                index++;
                std::cout << "mtlcolor: " << materials[index] << std::endl; 
            }
            else if (keys[0] == "sphere") {
                if (keys.size() != 5) {
                    std::cout << "4 numbers are required for spheres" << std::endl;
                    return 0;
                }
                Vector center = Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]), 1.0);
                float radius = stof(keys[4]);
                objects.push_back(Sphere(center, radius, index));
                std::cout << "sphere: " << center.x() << " " << center.y() << " " << center.z() << " " << radius << std::endl; 
            }
            else if (keys[0] == "parallel") {
                if (keys.size() != 2) {
                    std::cout << "1 number is required for parallel" << std::endl;
                    return 0;
                }
                frustum_w = stof(keys[1]);
                parallel = true;
                read_inputs++;
            }
            // for the lights the attenuation is set to 1.0, 0.0, 0.0 by default
            else if (keys[0] == "light") {
                if (keys.size() != 6) {
                    std::cout << "5 numbers are required for light" << std::endl;
                    return 0;
                }
                lights.push_back(Light(Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]), stof(keys[4])), stof(keys[5])));
            }
            else if (keys[0] == "attlight") {
                if (keys.size() != 9) {
                    std::cout << "8 numbers are required for attlight" << std::endl;
                    return 0;
                }
                Light l = Light(Vector(stof(keys[1]), stof(keys[2]), stof(keys[3]), stof(keys[4])), stof(keys[5]));
                l.set_att(stof(keys[6]), stof(keys[7]), stof(keys[8]));
                lights.push_back(l);
            }
            else if (keys[0] == "depthcueing"){
                if (keys.size() != 8) {
                    std::cout << "7 numbers are required for depthcueing" << std::endl;
                    return 0;
                }
                dc = Color(stof(keys[1]), stof(keys[2]), stof(keys[3]));
                depth_alpha[0] = stof(keys[4]);
                depth_alpha[1] = stof(keys[5]);
                depth_dist[0] = stof(keys[6]);
                depth_dist[1] = stof(keys[7]);
                std::cout << "depthcueing: " << dc << " alpha: " << depth_alpha[0] << "-" << depth_alpha[1] << " dist: " << depth_dist[0] << "-" << depth_dist[1] << std::endl;
            }
            else {
                std::cout << "invalid key: "  << keys[0] << std::endl;
                return 0;
            }
        }

        // a few checks to make sure the input is valid
        if (read_inputs < 6) {
            std::cout << "missing a required input: [eye, viewdir, updir, hfov/parallel, res, bkgcolor]" << std::endl;
            return 0;
        }
        else if (index == -1 || objects.size() < 1) {
            std::cout << "missing at least one mtlcolor, and/or one sphere" << std::endl;
            return 0;
        }
        if (parallel && frustum_w <= 0) {
            std::cout << "invalid format: parallel <frustum_width>" << std::endl;
            std::cout << "frustum width must be positive" << std::endl;
            return 0;
        }
        std::cout << "# of material colors: " << index + 1 << std::endl;
        std::cout << "# of objects: " << objects.size() << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid values in input file: " << e.what() << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 0;
    }

    int size = resolution[0] * resolution[1];
    Color *pixelmap = new Color[size];
    viewdir.normalize();
    updir.normalize();

    // viewdir cross updir approaching invalidity with fp error
    if (viewdir.dot(updir) < -0.9 || viewdir.dot(updir) > 0.9) {
        std::cout << "up vector is too close to view vector" << std::endl;
        return 0;
    } 

    // define the viewing coordinate system
    Vector u = viewdir.cross(updir);
    u.normalize();
    Vector v = u.cross(viewdir);

    float aspect = (float)resolution[0] / (float)resolution[1];
    float d = 1;

    // width is just frustum width for a parallel projection
    float width = (parallel) ? frustum_w : 2 * d * tan(hfov / 2);
    float height = width / aspect;

    // go to view plane then to the left/right edge, then to the top/bottom
    Vector ul = (eye + d * viewdir) - ((width / 2) * u) + ((height/2) * v);
    Vector ur = (eye + d *  viewdir) + ((width / 2) * u) + ((height/2) * v);

    Vector ll = (eye + d *  viewdir) - ((width / 2) * u) - ((height/2) * v);
    Vector lr = (eye + d *  viewdir) + ((width / 2) * u) - ((height/2) * v);

    Vector deltah = (1.0 / (resolution[0] - 1)) * (ur - ul);
    Vector deltav = (1.0 / (resolution[1] - 1)) * (ll - ul);

    // origin is ul if parallel
    Vector origin = (parallel) ? ul : eye;
    // direction is always viewdir if parallel
    Vector direction = (parallel) ? viewdir : ul - eye;

    Ray ray = Ray(origin, direction);

    // for every pixel in the output image trace a ray to get its color
    for (int i = 0; i < resolution[1]; i++) {
        // for parallel the direction is always the same
        if (parallel) {
            ray.set_origin((ul + (i * deltav)));
        }
        // for perspective origin is always the eye
        else {
            ray.set_direction((ul + i * deltav) - eye);
        }
        for (int j = 0; j < resolution[0]; j++){
            int pos = j + (resolution[0] * i);
            pixelmap[pos] = trace_ray(ray);
            
            if (parallel) {
                ray.set_origin((ul + (i * deltav) + (j * deltah)));
            }
            else {
                ray.set_direction((ul + (i * deltav) + (j * deltah)) - eye);
            }
        }
    }

    std::string filename = argv[1];
    filename.std::string::erase(filename.std::string::find("."));
    output.open(filename + ".ppm");

    if (!output.is_open()) {
        std::cout << "failed to create output file.";
        return 0;
    }

    // write colors to ppm file 
    output << "P3\n"
        << "#raytracingiscool\n"
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

    // cleanup
    output.close();
    delete [] pixelmap;
    lights.clear();
    return 1;
}