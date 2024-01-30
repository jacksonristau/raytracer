#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3 {
    public:
        Vector3();

        Vector3(float x, float y, float z);

        Vector3(const Vector3& v2);

        virtual ~Vector3();

        // equality
        bool operator==(const Vector3& v) const;

        // negation
        Vector3 operator-(const Vector3& v);

        // addition
        Vector3 operator+(const Vector3& v1);

        Vector3 operator=(const Vector3& v1);

        float x() const {return v[0];}
        float y() const {return v[1];}
        float z() const {return v[2];}

    private:
        float v[3];
};
#endif