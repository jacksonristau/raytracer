#ifndef LIGHT_H
#define LIGHT_H

// abstract base
class Light {
    public:
        Light(float intensity) : i(intensity) {}
        virtual ~Light() {}

        virtual float intensity() const { return i; }
        virtual float x() const = 0;
        virtual float y() const = 0;
        virtual float z() const = 0;
        virtual float w() const = 0;
    private:
        float i;
};

#endif 