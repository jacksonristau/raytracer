#include "../include/geometry/triangle.h"
#include "../include/geometry/plane.h"

Hit Triangle::intersect(const Ray& r) const {
    const Point3& v0 = mesh->vertices[index * 3 + 0];
    Point3 v1 = mesh->vertices[index * 3 + 1];
    Point3 v2 = mesh->vertices[index * 3 + 2];

    Vector3 e1 = v1 - v0;
    Vector3 e2 = v2 - v0;

    const Vector3& n = e1.cross(e2);
    float mag = n.magnitude();
    if (is_near_zero(mag))
        return Hit();

    Plane plane(v0, n);
    Hit h = plane.intersect(r);
    if (is_negative(h.t))
        return h;

    float d11 = e1.dot(e1);
    float d12 = e1.dot(e2);
    float d22 = e2.dot(e2);
    float det = (d11 * d22) - (d12 * d12);

    Vector3 ep = h.x_pos - v0;
    float dp1 = ep.dot(e1);
    float dp2 = ep.dot(e2);

    h.u     = ((d22 * dp1) - (d12 * dp2)) / det;
    h.v     = ((d11 * dp2) - (d12 * dp1)) / det;
    float a = 1.0f - (h.u + h.v);

    if (is_negative(a) || a >= 1.0f || is_negative(h.u) || h.u >= 1.0f || is_negative(h.v) || h.v >= 1.0f)
        return Hit();

    return h;
}

Vector3 Triangle::normal(const Hit& h, const Point3& p) const {
    if (mesh->normals.size() == 0) {
        // flat shaded
        return h.normal;
    }
    else {
        // interpolated normals
        float a = 1.0f - (h.u + h.v);
        int ni0 = mesh->normal_indices[index * 3 + 0];
        int ni1 = mesh->normal_indices[index * 3 + 1];
        int ni2 = mesh->normal_indices[index * 3 + 2];
        Vector3 n = a * mesh->normals[ni0] + h.u * mesh->normals[ni1] + h.v * mesh->normals[ni2];
        n.normalize();
        
        return n;
    }
}