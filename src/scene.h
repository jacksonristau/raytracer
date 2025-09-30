#pragma once

#include <string>
#include <vector>
#include "math/vector.h"
#include "color.h"
#include "material.h"
#include "sphere.h"
#include "light.h"
#include "texture.h"
#include "camera.h"
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
        Camera get_camera() const { return camera; }

        // these return lists of vertices, normals, and uvs for a given index into the indices array
        std::vector<Vector> get_vertices(int index) const;
        std::vector<Vector> get_normals(int index) const;
        std::vector<std::vector<float>> get_uvs(int index) const;
        

        auto num_materials() const { return materials.size();}
        auto num_spheres() const { return spheres.size();}
        auto num_lights() const { return lights.size();}
        auto num_vertices() const { return vertices.size();}
        auto num_normals() const { return normals.size();}
        auto num_indices() const { return vertex_indices.size();}
        auto num_uvs() const { return uvs.size();}

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

        Camera camera;

        Color bkgcolor;
        float bkgeta;
};