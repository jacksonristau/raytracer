#ifndef LIGHT_H
#define LIGHT_H

class Light {
    public:
        Light(Vector v, float intensity) : i(intensity), v(v) {}
        virtual ~Light() {}

        float intensity() const { return i; }
        Vector l() const { return v; }
        float w() const { return v.w(); }
    private:
        const Vector v;
        float i;
};

#endif 