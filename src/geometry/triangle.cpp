#include "../include/geometry/triangle.h"
#include "../include/geometry/plane.h"
#include "../include/geometry/aabb.h"

Point3 Triangle::get_vertex(int v) const {
    tinyobj::index_t idx = mesh->indices[index + v];
    std::vector<tinyobj::real_t> vertices = mesh->get_vertices();

    tinyobj::real_t vx = vertices.at(3 * size_t(idx.vertex_index) + 0);
    tinyobj::real_t vz = vertices.at(3 * size_t(idx.vertex_index) + 2);
    tinyobj::real_t vy = vertices.at(3 * size_t(idx.vertex_index) + 1);
    return Point3(vx, vy, vz);
}

bool Triangle::has_normal() const {
    tinyobj::index_t idx = mesh->indices[index];
    return idx.normal_index >= 0;
}

bool Triangle::has_uv() const {
    tinyobj::index_t idx = mesh->indices[index];
    return idx.texcoord_index >= 0;
}

Vector3 Triangle::get_normal(int n) const {
    std::vector<tinyobj::real_t> normals = mesh->get_normals();

    tinyobj::index_t idx = mesh->indices[index + n];
    tinyobj::real_t nx = normals.at(3 * size_t(idx.normal_index) + 0);
    tinyobj::real_t ny = normals.at(3 * size_t(idx.normal_index) + 1);
    tinyobj::real_t nz = normals.at(3 * size_t(idx.normal_index) + 2);

    return Vector3(nx, ny, nz);
}

Point2 Triangle::get_uv(int t) const {
    std::vector<tinyobj::real_t> uvs = mesh->get_uvs();

    tinyobj::index_t idx = mesh->indices[index + t];
    tinyobj::real_t u = uvs.at(2 * size_t(idx.texcoord_index) + 0);
    tinyobj::real_t v = uvs.at(2 * size_t(idx.texcoord_index) + 1);
    
    return Point2(u, v);
}

Hit Triangle::intersect(const Ray& r, bool with_uv) const {
    Point3 v0 = get_vertex(0);
    Point3 v1 = get_vertex(1);
    Point3 v2 = get_vertex(2);

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