#include "color.h"
#include "ray.h"
#include "scene.h"
Scene scene;
const float epsilon = 0.00001;

// trace ray
/*
- input: incident ray
- output: pixel color

for each object in the scene determine if the incident ray
intersects and keep only the closest positive intersection point

if a valid intersection exists call shade_ray to determine the 
color at the intersection point.

*/
Color trace_ray(Ray ray, bool entering) {
    float min_t = INFINITY;
    Color output = scene.background();
    int hit = -1;
    bool is_sphere = false;
    float t = -1.0;

    for (int i = 0; i < scene.num_spheres(); i++) {
        t = ray.intersect_sphere(scene.get_sphere(i));
        if (t > 0 && t < min_t){
            min_t = t;
            hit = i;
            is_sphere = true;
        }
    }
    float bary[3];
    for (int i = 0; i < scene.num_indices(); i++) {
        // bary is filled with the barycentric coordinates of the intersection
        t = ray.intersect_triangle(scene.get_vertices(i), bary);
        if (t > 0 && t < min_t){
            min_t = t;
            hit = i;
            is_sphere = false;
        }
    }
}

// shade ray
/*
- input: intersection information
- output: color at intersection

computes color at intersection using phong illumination

phong req: normal, material, intersection point

reflection rq: incident ray

*/

Vector get_normal(Vector x, int o, float* bary) {
    Vector n;
    if (bary == NULL) {
        n = x - scene.get_sphere(o).center();
        n = (1 / scene.get_sphere(o).radius()) * n;
    }
    else {
        std::vector<Vector> normals = scene.get_normals(o);
        if (normals.size() == 0){
            std::vector<Vector> vertices = scene.get_vertices(o);
            n = (vertices[1] - vertices[0]).cross(vertices[2] - vertices[0]);
            n.normalize();
        }
        else {
            n = bary[0] * normals[0] + bary[1] * normals[1] + bary[2] * normals[2];
            n.normalize();
        }
    }
    return n;
}

void get_uv(Vector x_p, int o, const float* bary, float* uv) {
    if (bary == NULL) {
        scene.get_sphere(o).get_uv(x_p, uv);
    }
    else {
        std::vector<std::vector<float>> uvs = scene.get_uvs(o);
        uv[0] = (bary[0] * uvs[0][0]) + (bary[1] * uvs[1][0]) + (bary[2] * uvs[2][0]);
        uv[1] = (bary[0] * uvs[0][1]) + (bary[1] * uvs[1][1]) + (bary[2] * uvs[2][1]);
    }
}


Color shade_ray(int m, int o, Vector x_p, Ray i_ray, bool entering, float* bary) {
    Material mat = scene.get_material(m);
    Vector n = get_normal(x_p, o, bary);
    Color final_color = Color();
    if (mat.has_texture()){
        float uv[2];
        get_uv(x_p, o, bary, uv);
        final_color = mat.ka() * scene.get_texture_color(mat.get_texture(), uv[0], uv[1]);
    }
    else {
        final_color = mat.ka() * mat.diffuse();
    }
    float s_flag = calculate_sflag(x_p, o, bary);
    
    float ndotl = std::max(0.0f, n.dot(l));
    float ndoth;
    if (ndotl == 0.0) {
        ndoth = 0.0;
    }
    else {
        Vector h = l + (-i_ray.direction());
        h.normalize();
        ndoth = std::max(0.0f, n.dot(h));
    }

    Color diffuse = ndotl * mat.kd() * d_lambda;
    Color specular = std::pow(ndoth, mat.n()) * mat.ks() * mat.specular();
}