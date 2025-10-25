#pragma once
#include "math/vector3.h"
#include "math/ray.h"

class ILight {
    public:
        virtual ~ILight() {};

        virtual float atten(float d) const = 0;

        virtual Ray get_shadow_ray(const Point3& x_p) const = 0;
        virtual void print(std::ostream& os) const = 0;
        virtual bool is_point() const = 0;
        virtual float dist(const Point3& p) const = 0;
        virtual float intensity() const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const ILight& light) {
    light.print(os);
    return os;
}

class DirectionalLight : public ILight {
public:
    DirectionalLight() : l(Vector3(0.0f, 0.0f, -1.0f)), i(1.0f), c0(1.0f), c1(0.0f), c2(0.0f) {}

    DirectionalLight(Vector3 l, float i) : l(l), i(i), c0(1.0f), c1(0.0f), c2(0.0f) {
        l.normalize(); 
    }

    DirectionalLight(Vector3 l, float i, float c0, float c1, float c2) : l(l), i(i), c0(c0), c1(c1), c2(c2) {
        l.normalize(); 
    }

    ~DirectionalLight() {}

    void print(std::ostream& os) const override {
        os << "directional light: <" << l.x << ", " << l.y << ", " << l.z << ">, i: " << i;
    }

    Ray get_shadow_ray(const Point3& x_p) const override { return Ray(x_p, l); }
    bool is_point() const override { return false; }
    float dist(const Point3& p) const override { return INFINITY; }
    float intensity() const override { return i; }

    float atten(float d) {
        return 1.0f / (c0 + c1 * d + c2 * d * d);
    }

private:
    Vector3 l;

    float i;

    float c0;
    float c1;
    float c2;
};

class PointLight : public ILight {
public:
    PointLight() : l(Point3(0.0f, 0.0f, 2.0f)), i(1.0f), c0(1.0f), c1(0.0f), c2(0.0f) {}

    PointLight(Point3 l, float i) : l(l), i(i), c0(1.0f), c1(0.0f), c2(0.0f) {}

    PointLight(Point3 l, float i, float c0, float c1, float c2) : l(l), i(i), c0(c0), c1(c1), c2(c2) {}

    ~PointLight() {}

    void print(std::ostream& os) const override {
        os << "point light: (" << l.x << ", " << l.y << ", " << l.z << "), i: " << i;
    }

    float dist(const Point3& x_p) const override { return distance(x_p, l); }
    bool is_point() const override { return true; }
    float intensity() const override { return i; }
    Ray get_shadow_ray(const Point3& x_p) const override { return Ray(x_p, l - x_p); }

    float atten(float d) {
        return 1.0f / (c0 + c1 * d + c2 * d * d);
    }

private:
    Point3 l;

    float i;

    float c0;
    float c1;
    float c2;
};