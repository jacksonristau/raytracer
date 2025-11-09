#include <fstream>
#include <iostream>
#include <cmath>

#include "../include/gfx/material.h"
#include "../include/gfx/lights.h"
#include "../include/gfx/scene.h"
#include "../include/gfx/raytracer.h"
#include "../include/geometry/hit.h"

BPMaterial::BPMaterial() {
    texture = std::shared_ptr<BPColorTexture>(new BPColorTexture(Color()));
    specular = Color(1.0f, 1.0f, 1.0f);
    k[0] = 0.3f;
    k[1] = 0.6f;
    k[2] = 0.3f;
    n_val = 50;
    a = 1.0f;
    refractive_index = 1.0f;
    precomp[0] = k[0] * texture->get_pixel(0.0f, 0.0f);
    precomp[1] = k[1] * texture->get_pixel(0.0f, 0.0f);
    precomp[2] = k[2] * specular;
}

BPMaterial::BPMaterial(std::shared_ptr<Texture> t, float ka, float kd, float ks, int n, float alpha, float eta) {
    texture = t;
    k[0] = ka;
    k[1] = kd;
    k[2] = ks;
    n_val = n;
    a = alpha;
    refractive_index = eta;
    if (texture->is_uniform()) {
        precomp[0] = k[0] * texture->get_pixel(0.0f, 0.0f);
        precomp[1] = k[1] * texture->get_pixel(0.0f, 0.0f);
        precomp[2] = k[2] * specular;
    }
}

BPMaterial::BPMaterial(json material_json) {
    specular = Color(1.0f, 1.0f, 1.0f);
    k[0] = 0.3f;
    k[1] = 0.6f;
    k[2] = 0.3f;
    n_val = 50;
    a = 1.0f;
    refractive_index = 1.0f;
    try {
        if (material_json.contains("imagetexture")) {
            std::string fn = material_json.at("imagetexture");
            texture = std::make_shared<ImageTexture>(fn);
        }
        else if (material_json.contains("colortexture")) {
            Color c(material_json.at("colortexture"));
            texture = std::make_shared<BPColorTexture>(c);
        }
        else if (material_json.contains("diffuse")) {
            Color c(material_json.at("diffuse"));
            texture = std::make_shared<BPColorTexture>(c);
        }
        else {
            // fallback to a uniform black/zero diffuse
            texture = std::make_shared<BPColorTexture>(Color());
        }

        if (material_json.contains("specular")) {
            specular = Color(material_json.at("specular"));
        }

        if (material_json.contains("ka")) k[0] = material_json.at("ka");
        if (material_json.contains("kd")) k[1] = material_json.at("kd");
        if (material_json.contains("ks")) k[2] = material_json.at("ks");
        if (material_json.contains("n")) n_val = material_json.at("n");
        if (material_json.contains("alpha")) a = material_json.at("alpha");
        if (material_json.contains("eta")) refractive_index = material_json.at("eta");
    }
    catch (const json::exception& e) {
        std::cerr << "warning: malformed material json: " << e.what() << '\n';
    }
    if (texture && texture->is_uniform()) {
        Color base = texture->get_pixel(0.0f, 0.0f);
        precomp[0] = k[0] * base;
        precomp[1] = k[1] * base;
        precomp[2] = k[2] * specular;
    }
}

BPMaterial BPMaterial::operator=(const BPMaterial& m1) {
    texture = m1.texture;
    k[0] = m1.k[0];
    k[1] = m1.k[1];
    k[2] = m1.k[2];
    n_val = m1.n_val;
    a = m1.a;
    refractive_index = m1.refractive_index;
    if (texture->is_uniform()) {
        precomp[0] = m1.precomp[0];
        precomp[1] = m1.precomp[1];
        precomp[2] = m1.precomp[2];
    }
    return *this;
}

Color BPMaterial::get_diffuse(const Hit& hit, float ndotl) const {
    if (ndotl == 0.0f)
        return Color(0.0f, 0.0f, 0.0f);

    if (texture->is_uniform())
        return ndotl * precomp[1];

    return ndotl * texture->get_pixel(hit.u, hit.v);
}

Color BPMaterial::evaluate(const Hit& hit, Color reflection, Color transmission) const {
    Color dlambda = texture->get_pixel(hit.u, hit.v);
    Color final_color = texture->is_uniform() ? precomp[0] : k[0] * dlambda;
    for (const ILight* light : Scene::lights) {
        Ray shadow_ray = light->get_shadow_ray(hit);
        float d = light->dist(hit.x_pos);
        float shadow_mask = Raytracer::trace_shadow_ray_naive(shadow_ray, light, d);
        
        Vector3 v(hit.r.direction);
        v = -v;
        Vector3 l(shadow_ray.direction);
        

        float ndotl = std::max(0.0f, hit.normal.dot(l));
        float ndoth = 0.0f;
        if (ndotl != 0.0f) {
            Vector3 h = l + v;
            h.normalize();
            ndoth = std::max(0.0f, hit.normal.dot(h));
        }
        Color diffuse, specular;
        if (texture->is_uniform()) {
            diffuse = ndotl == 0.0f ? Color(0.0f, 0.0f, 0.0f) : ndotl * precomp[1];
            specular = ndotl == 0.0f ? Color(0.0f, 0.0f, 0.0f) : std::pow(ndoth, n_val) * precomp[2];
        }
        else {
            diffuse = ndotl == 0.0f ? Color(0.0f, 0.0f, 0.0f) : ndotl * k[1] * dlambda;
            specular = ndotl == 0.0f ? Color(0.0f, 0.0f, 0.0f) : std::pow(ndoth, n_val) * k[2] * specular;
        }
        float i = light->intensity() * light->atten(d);
        final_color = final_color + (i * shadow_mask * (diffuse + specular + reflection + transmission));
    }
    final_color = Scene::camera.depth_cue(hit.x_pos, final_color);
    return final_color;
}