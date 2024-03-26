#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "vector.h"
#include "color.h"
#include "material.h"
#include "sphere.h"
#include "light.h"
#include "texture.h"
#include <memory>

class Scene {
    public:
        Scene();
        ~Scene();

        int load_from_file(const std::string& filename);
        Color get_texture_color(int index, float u, float v) const;

        Color background() const { return bkgcolor;}
        float eta() const { return bkgeta;}
        
        Material get_material(int index) const { return materials[index]; }
        int get_material_index(int index) const { return material_indices[index];}
        Sphere get_sphere(int index) const { return spheres[index];}
        Light get_light(int index) const { return lights[index];}

        // these return lists of vertices, normals, and uvs for a given index into the indices array
        std::vector<Vector> get_vertices(int index) const;
        std::vector<Vector> get_normals(int index) const;
        std::vector<std::vector<float>> get_uvs(int index) const;
        

        int num_materials() const { return materials.size();}
        int num_spheres() const { return spheres.size();}
        int num_lights() const { return lights.size();}
        int num_vertices() const { return vertices.size();}
        int num_normals() const { return normals.size();}
        int num_indices() const { return vertex_indices.size();}
        int num_uvs() const { return uvs.size();}

        Vector eye() const { return eye_pos;}
        Vector view() const { return viewdir;}
        Vector up() const { return updir;}

        float fov() const { return hfov;}
        float frustum_width() const { return frustum_w;}
        bool is_parallel() const { return parallel;}

        inline int px_width() const { return resolution[0];}
        inline int px_height() const { return resolution[1];}

        Color dc_color() const { return dc;}
        float alpha_min() const { return alpha[0];}
        float alpha_max() const { return alpha[1];}
        float dist_min() const { return dist[0];}
        float dist_max() const { return dist[1];}

        Color depth_cue(Vector x, Color i, float view_dist) const;

    private:
        static std::vector<std::string> split(std::string in, char delim);

        std::vector<Material> materials;
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<Sphere> spheres;
        std::vector<Light> lights;

        // these all have the same length
        std::vector<Vector> vertices;
        std::vector<Vector> normals;
        std::vector<std::vector<float>> uvs;

        // these all have the same length and correspond at each index i
        std::vector<std::vector<int>> vertex_indices;
        std::vector<std::vector<int>> texture_indices;
        std::vector<std::vector<int>> normal_indices;
        // act like the material field in a sphere but for triangles
        std::vector<int> material_indices;

        Vector eye_pos;
        Vector viewdir;
        Vector updir;

        float hfov;
        int resolution[2];
        Color bkgcolor;
        float bkgeta;

        // parallel projection
        float frustum_w = -1.0;
        bool parallel = false;

        // depth cueing
        Color dc = Color(-1, -1, -1);
        float alpha[2];
        float dist[2];
};

#endif