#include "../include/geometry/mesh.h"
#include "../include/math/matrix4x4.h"

void Mesh::build(const Matrix4x4& obj_to_world) {
    const auto& verts = get_vertices();
    const auto& norms = get_normals();
    const auto& texcoords = get_uvs();

    size_t num_face_verts = ntris * 3;
    vertices.resize(num_face_verts * 3);

    has_normals = !norms.empty() && indices[0].normal_index >= 0;
    has_uvs = !texcoords.empty() && indices[0].texcoord_index >= 0;

    if (has_normals) normals.resize(num_face_verts * 3);
    if (has_uvs) uvs.resize(num_face_verts * 2);

    for (size_t i = 0; i < num_face_verts; i++) {
        tinyobj::index_t idx = indices[i];

        // resolve vertex, apply y/z swap here, transform to world space
        float vx = verts[3 * idx.vertex_index + 0];
        float vy = verts[3 * idx.vertex_index + 1];
        float vz = verts[3 * idx.vertex_index + 2];
        Point3 p = obj_to_world.transform_point(Point3(vx, vy, vz));
        vertices[i * 3 + 0] = p.x;
        vertices[i * 3 + 1] = p.y;
        vertices[i * 3 + 2] = p.z;

        if (has_normals) {
            float nx = norms[3 * idx.normal_index + 0];
            float ny = norms[3 * idx.normal_index + 1];
            float nz = norms[3 * idx.normal_index + 2];
            Vector3 n = obj_to_world.transform_vector(Vector3(nx, ny, nz));
            n.normalize();
            normals[i * 3 + 0] = n.x;
            normals[i * 3 + 1] = n.y;
            normals[i * 3 + 2] = n.z;
        }

        if (has_uvs) {
            uvs[i * 2 + 0] = texcoords[2 * idx.texcoord_index + 0];
            uvs[i * 2 + 1] = texcoords[2 * idx.texcoord_index + 1];
        }
    }
    attrib.reset();
    indices.clear();
    indices.shrink_to_fit();
}