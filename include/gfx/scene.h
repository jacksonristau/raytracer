#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../math/vector3.h"
#include "../geometry/primitive.h"

#include "color.h"
#include "material.h"
#include "lights.h"
#include "texture.h"
#include "camera.h"


class Scene {
    public:
        Scene();
        ~Scene();

        int load_from_file(const std::string& filename);
        Color get_texture_color(int index, float u, float v) const;
        
        //Material get_material(int index) const { return materials[index]; }
        ILight* get_light(int index) const { return lights[index];}


        // these return lists of vertices, normals, and uvs 
        // for a given index into the indices array
        
        //std::vector<Point3> get_vertices(int index) const;
        //std::vector<Vector3> get_normals(int index) const;
        //std::vector<std::vector<float>> get_uvs(int index) const;
        
        static Camera camera;

        static const Color bkgcolor;
        static const float bkgeta;
        static std::vector<ILight*> lights;
        static std::vector<Primitive> primitives;
    private:
        static std::vector<std::string> split(std::string in, char delim);

        std::vector<Material> materials;
        std::vector<std::shared_ptr<Texture>> textures;

        //// these all have the same length
        //std::vector<Point3> vertices;
        //std::vector<Vector3> normals;
        //std::vector<std::vector<float>> uvs;

        //// these all have the same length and correspond at each index i
        //std::vector<std::vector<int>> vertex_indices;
        //std::vector<std::vector<int>> texture_indices;
        //std::vector<std::vector<int>> normal_indices;
        //// act like the material field in a sphere but for triangles
        //std::vector<int> material_indices;
};