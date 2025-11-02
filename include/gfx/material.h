#pragma once

#include <ostream>
#include <vector>
#include <string>
#include "color.h"
#include "texture.h"
class Color;
class Hit;

// holds information for doing blinn-phong lighting calculations
class Material {
    public:
        virtual Color evaluate(const Hit& hit, Color reflection, Color transmission) const = 0;
        virtual float alpha() const = 0;
        virtual float eta() const = 0;
        virtual bool is_glossy() const = 0;
        virtual bool is_transparent() const = 0;
};

class BPMaterial : public Material {
    public:
        BPMaterial();
        BPMaterial(std::shared_ptr<Texture> t, float ka, float kd, float ks, int n, float alpha, float eta);
        BPMaterial(json material_json);

        BPMaterial operator=(const BPMaterial& m1);

        Color evaluate(const Hit& hit, Color reflected, Color refracted) const override;
        float alpha() const override { return a; };
        float eta() const override { return refractive_index; }
        bool is_glossy() const override { return k[2] > 0.0f; }
        bool is_transparent() const override { return a < 1.0f; }

        Color get_diffuse(const Hit& hit, float ndotl) const;

    private:
        std::shared_ptr<Texture> texture;
        Color specular;
        float k[3];
        Color precomp[3];
        float refractive_index;
        float a;
        int n_val;

};

//// Simple non-thread-safe factory for creating lights from json
//class MaterialFactory {
//public:
//    // Returns a new heap-allocated ILight* based on the json description.
//    // Caller is responsible for deleting the returned pointer (or managing with a smart pointer).
//    static Material* create(const json& light_json);
//};