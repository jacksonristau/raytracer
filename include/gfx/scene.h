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
        
        static Camera camera;

        static Color bkgcolor;
        static float bkgeta;
        static std::vector<ILight*> lights;
        static std::vector<Primitive> primitives;
    private:
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