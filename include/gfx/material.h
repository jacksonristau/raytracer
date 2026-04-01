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
        virtual Color evaluate_debug(const Hit& hit, Color reflection, Color transmission) const = 0;
        virtual float alpha() const = 0;
        virtual float eta() const = 0;
        virtual bool is_uniform() const = 0;
        virtual bool is_glossy() const = 0;
        virtual bool is_transparent() const = 0;
        virtual bool get_color() const = 0;
};

class ToonMaterial : public Material {
    public:
        ToonMaterial();
        ToonMaterial(json material_json);
        ToonMaterial(std::shared_ptr<ImageTexture> t);

        Color evaluate(const Hit& hit, Color reflected, Color refracted) const override;
        Color evaluate_debug(const Hit& hit, Color reflection, Color transmission) const override;
        float alpha() const override { return 1.0f; };
        float eta() const override { return 1.0f; }
        bool is_glossy() const override { return false; }
        bool is_transparent() const override { return false; }
        bool is_uniform() const override { return true; }
        bool get_color() const override { return false; }
    private:
        std::shared_ptr<Texture> ramp;
};

class BPMaterial : public Material {
    public:
        BPMaterial();
        BPMaterial(std::shared_ptr<Texture> t, float ka, float kd, float ks, int n, float alpha, float eta);
        BPMaterial(json material_json);

        BPMaterial operator=(const BPMaterial& m1);

        Color evaluate(const Hit& hit, Color reflected, Color refracted) const override;
        Color evaluate_debug(const Hit& hit, Color reflection, Color transmission) const override;
        float alpha() const override { return a; };
        float eta() const override { return refractive_index; }
        bool is_glossy() const override { return k[2] > 0.0f; }
        bool is_transparent() const override { return a < 1.0f; }
        bool is_uniform() const override {return texture->is_uniform();}
        bool get_color() const override { return texture->get_pixel(0.0f, 0.0f).g == 1.0; }

        Color get_diffuse(const Hit& hit, float ndotl) const;

    private:
        std::shared_ptr<Texture> texture;
        Color spec;
        float k[3];
        Color precomp[3];
        float refractive_index;
        float a;
        int n_val;

};