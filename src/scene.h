#ifndef SCENE_H
#define SCENE_H

#include <string>
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
        Sphere get_object(int index) const { return objects[index];}
        Light get_light(int index) const { return lights[index];}

        int num_materials() const { return materials.size();}
        int num_objects() const { return objects.size();}
        int num_lights() const { return lights.size();}

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
        std::vector<Sphere> objects;
        std::vector<Light> lights;

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