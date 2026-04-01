#include "../include/geometry/triangle.h"
#include "../include/geometry/plane.h"
#include "../include/geometry/aabb.h"
#include "../include/gfx/raytracer.h"

Point3 Triangle::get_vertex(int v) const {
    size_t i = (index + v) * 3;
    return Point3(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
}

Vector3 Triangle::get_normal(int n) const {
    size_t i = (index + n) * 3;
    return Vector3(mesh->normals[i], mesh->normals[i + 1], mesh->normals[i + 2]);
}

Point2 Triangle::get_uv(int t) const {
    size_t i = (index + t) * 2;
    return Point2(mesh->uvs[i], mesh->uvs[i + 1]);
}

bool Triangle::has_normal() const { return mesh->has_normals; }
bool Triangle::has_uv() const { return mesh->has_uvs; }

Hit Triangle::intersect_old(const Ray& r, bool with_uv) const {
    Point3 v0 = get_vertex(0);
    Point3 v1 = get_vertex(1);
    Point3 v2 = get_vertex(2);

    Vector3 e1 = v1 - v0;
    Vector3 e2 = v2 - v0;

    const Vector3& n = e1.cross(e2);
    float mag = n.magnitude();
    float angle = mag / (e1.magnitude() * e2.magnitude());
    // degen triangle
    if (is_near_zero(angle))
        return Hit();

    // backface culling
    //if (n.dot(r.direction) > 0)
    //    return Hit();

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

    float beta     = (((d22 * dp1) - (d12 * dp2)) / det);
    float gamma    = (((d11 * dp2) - (d12 * dp1)) / det);

    float alpha = 1.0f - (beta + gamma);

    // exit if the point is outside the triangle
    if (is_negative(alpha) || alpha >= 1.0f || is_negative(beta) || beta >= 1.0f || is_negative(gamma) || gamma >= 1.0f)
        return Hit();

    float edge_threshhold = 0.01f;
    if (alpha < edge_threshhold || beta < edge_threshhold || gamma < edge_threshhold)
        h.is_edge = true;

    if (!has_uv()) {
        Point2 uv = compute_planar_uv(h.x_pos, h.normal);
        h.u = uv.x;
        h.v = uv.y;
    } else {
        Point2 uv0 = get_uv(0);
        Point2 uv1 = get_uv(1);
        Point2 uv2 = get_uv(2);

        h.u = alpha * uv0.x + beta * uv1.x + gamma * uv2.x;
        h.v = alpha * uv0.y + beta * uv1.y + gamma * uv2.y;
    }
    if (has_normal()){
        Vector3 n0 = get_normal(0);
        Vector3 n1 = get_normal(1);
        Vector3 n2 = get_normal(2);

        Vector3 n = alpha * n0 + beta * n1 + gamma * n2;
        n.normalize();
        h.normal = n;
    }
    // simple planar normal already set for h.normal

    return h;
}

// moller trombore
Hit Triangle::intersect(const Ray& r, bool with_uv) const {
    Point3 v0 = get_vertex(0);
    Point3 v1 = get_vertex(1);
    Point3 v2 = get_vertex(2);

    Vector3 e1 = v1 - v0;
    Vector3 e2 = v2 - v0;

    Vector3 h_vec = r.direction.cross(e2);
    float a = e1.dot(h_vec);

    if (a > -Eps && a < Eps)
        return Hit();

    float f = 1.0f / a;
    Vector3 s = r.origin - v0;
    float beta = f * s.dot(h_vec);
    if (is_negative(beta, Eps * 128) || beta >= 1.0f)
        return Hit();

    Vector3 q = s.cross(e1);
    float gamma = f * r.direction.dot(q);
    if (is_negative(gamma, Eps * 128) || beta + gamma >= 1.0f)
        return Hit();

    float t = f * e2.dot(q);
    if (is_negative(t, Eps * 128))
        return Hit();

    float alpha = 1.0f - (beta + gamma);
    Point3 x_pos = r.get_point(t);
    Vector3 face_normal = e1.cross(e2);
    face_normal.normalize();

    Hit hit;
    hit.t = t;
    hit.r = r;
    hit.x_pos = x_pos;
    hit.normal = face_normal;
    hit.primitive = nullptr;

    if (alpha < Raytracer::edge_threshold || beta < Raytracer::edge_threshold || gamma < Raytracer::edge_threshold)
        hit.is_edge = true;

    if (!with_uv && !has_uv() && !has_normal())
        return hit;

    if (!has_uv()) {
        Point2 uv = compute_planar_uv(x_pos, face_normal);
        hit.u = uv.x;
        hit.v = uv.y;
    } else {
        Point2 uv0 = get_uv(0);
        Point2 uv1 = get_uv(1);
        Point2 uv2 = get_uv(2);
        hit.u = alpha * uv0.x + beta * uv1.x + gamma * uv2.x;
        hit.v = alpha * uv0.y + beta * uv1.y + gamma * uv2.y;
    }

    if (has_normal()) {
        Vector3 n0 = get_normal(0);
        Vector3 n1 = get_normal(1);
        Vector3 n2 = get_normal(2);
        Vector3 n = alpha * n0 + beta * n1 + gamma * n2;
        n.normalize();
        hit.normal = n;
    }

    return hit;
}

Point2 Triangle::compute_planar_uv(const Point3& p, const Vector3& normal) const {
    Vector3 abs_n(std::abs(normal.x), std::abs(normal.y), std::abs(normal.z));

    // Choose projection plane based on dominant axis
    if (abs_n.x > abs_n.y && abs_n.x > abs_n.z) {
        // Project onto YZ plane
        return Point2(p.y, p.z);
    } else if (abs_n.y > abs_n.z) {
        // Project onto XZ plane
        return Point2(p.x, p.z);
    } else {
        // Project onto XY plane
        return Point2(p.x, p.y);
    }
}

AABB Triangle::get_aabb() const {
    Point3 min = Point3::infinity();
    Point3 max = Point3::negative_infinity();

    Point3 v0 = get_vertex(0);
    Point3 v1 = get_vertex(1);
    Point3 v2 = get_vertex(2);


    min = Point3::min(min, v0);
    min = Point3::min(min, v1);
    min = Point3::min(min, v2);

    max = Point3::max(max, v0);
    max = Point3::max(max, v1);
    max = Point3::max(max, v2);

    return AABB(min, max);
}

Point3 Triangle::get_centroid() const {
    Point3 v0 = get_vertex(0);
    Point3 v1 = get_vertex(1);
    Point3 v2 = get_vertex(2);

    float m0 = (v0.x + v1.x + v2.x) / 3.0f;
    float m1 = (v0.y + v1.y + v2.y) / 3.0f;
    float m2 = (v0.z + v1.z + v2.z) / 3.0f;

    return Point3(m0, m1, m2);
}