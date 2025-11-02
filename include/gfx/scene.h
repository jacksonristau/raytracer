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
        Scene(json scene_desc);
        ~Scene();

        // make sure these get updated
        inline static Color bkgcolor = Color();
        inline static float bkgeta = 1.000293;

        static Camera camera;
        static std::vector<ILight*> lights;
        static std::vector<Primitive> primitives;
        std::vector<Material*> materials;
};