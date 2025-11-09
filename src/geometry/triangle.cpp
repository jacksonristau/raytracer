#include "../include/geometry/triangle.h"
#include "../include/geometry/plane.h"

Point3 Triangle::get_vertex(int v) const {
    tinyobj::index_t idx = mesh->indices[index + v];
    std::vector<tinyobj::real_t> vertices = mesh->get_vertices();

    tinyobj::real_t vx = vertices.at(3 * size_t(idx.vertex_index) + 0);
    tinyobj::real_t vz = vertices.at(3 * size_t(idx.vertex_index) + 2);
    tinyobj::real_t vy = vertices.at(3 * size_t(idx.vertex_index) + 1);
    return Point3(vx, vy, vz);
}

Vector3 Triangle::get_normal(int n) const {
    tinyobj::index_t idx = mesh->indices[index + n];
    if (idx.normal_index < 0)
        throw "no normal data";

    std::vector<tinyobj::real_t> normals = mesh->get_normals();

    tinyobj::real_t nx = normals.at(3 * size_t(idx.normal_index) + 0);
    tinyobj::real_t ny = normals.at(3 * size_t(idx.normal_index) + 1);
    tinyobj::real_t nz = normals.at(3 * size_t(idx.normal_index) + 2);
    return Vector3(nx, ny, nz);
}

Point2 Triangle::get_uv(int t) const {
    tinyobj::index_t idx = mesh->indices[index + t];

    if (idx.texcoord_index < 0)
        throw "no uv data";

    std::vector<tinyobj::real_t> uvs = mesh->get_uvs();

    if (idx.texcoord_index >= 0) {
        tinyobj::real_t u = uvs.at(2 * size_t(idx.texcoord_index) + 0);
        tinyobj::real_t v = uvs.at(2 * size_t(idx.texcoord_index) + 1);
        return Point2(u, v);
	}
    else {
		throw "no uvs defined for this mesh";
    }
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

    h.u     = ((d22 * dp1) - (d12 * dp2)) / det;
    h.v     = ((d11 * dp2) - (d12 * dp1)) / det;
    float a = 1.0f - (h.u + h.v);

    if (is_negative(a) || a >= 1.0f || is_negative(h.u) || h.u >= 1.0f || is_negative(h.v) || h.v >= 1.0f)
        return Hit();
    h.normal = normal(h, h.x_pos);
    return h;
}

Vector3 Triangle::normal(const Hit& h, const Point3& p) const {
    try {
        Vector3 n0 = get_normal(0);
        Vector3 n1 = get_normal(1);
        Vector3 n2 = get_normal(2);

        float a = 1.0f - (h.u + h.v);
        Vector3 n = a * n0 + h.u * n1 + h.v * n2;
        n.normalize();
        return n;
    }
    catch (const char* e) {
        return h.normal;
    }
}