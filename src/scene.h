#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "vector.h"
#include "color.h"
#include "material.h"
#include "sphere.h"
#include "light.h"

class Scene {
    public:
        Scene();
        ~Scene();

        int load_from_file(const std::string& filename);

        Color background() const { return bkgcolor;}
        
        Material get_material(int index) const { return materials[index]; }
        int get_material_index(int index) const { return material_indices[index];}
        Sphere get_sphere(int index) const { return spheres[index];}
        Light get_light(int index) const { return lights[index];}

        Vector get_vertex(int index) const { return vertices[index];}
        Vector get_normal(int index) const { return normals[index];}
        std::vector<int> get_indices(int index) const { return indices[index];}
        std::vector<float> get_uv(int index) const { return uvs[index];}

        int num_materials() const { return materials.size();}
        int num_spheres() const { return spheres.size();}
        int num_lights() const { return lights.size();}

        int num_vertices() const { return vertices.size();}
        int num_normals() const { return normals.size();}
        int num_indices() const { return indices.size();}
        int num_uvs() const { return uvs.size();}

        Vector eye() const { return eye_pos;}
        Vector view() const { return viewdir;}
        Vector up() const { return updir;}

        float fov() const { return hfov;}
        float frustum_width() const { return frustum_w;}
        bool is_parallel() const { return parallel;}

        int px_width() const { return resolution[0];}
        int px_height() const { return resolution[1];}

        Color dc_color() const { return dc;}
        float alpha_min() const { return alpha[0];}
        float alpha_max() const { return alpha[1];}
        float dist_min() const { return dist[0];}
        float dist_max() const { return dist[1];}

        Color depth_cue(Vector x, Color i, float view_dist) const;

    private:
        static std::vector<std::string> split(std::string in, char delim);

        std::vector<Material> materials;
        std::vector<Sphere> spheres;
        std::vector<Light> lights;

        std::vector<Vector> vertices;
        std::vector<Vector> normals;
        std::vector<std::vector<float>> uvs;

        std::vector<std::vector<int>> indices;
        std::vector<int> material_indices;

        Vector eye_pos;
        Vector viewdir;
        Vector updir;

        float hfov;
        int resolution[2];
        Color bkgcolor;

        // parallel projection
        float frustum_w = -1.0;
        bool parallel = false;

        // depth cueing
        Color dc = Color(-1, -1, -1);
        float alpha[2];
        float dist[2];
};

#endif