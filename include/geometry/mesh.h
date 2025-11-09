#pragma once

#include <vector>
#include "../include/math/vector3.h"
#include "../include/math/point2.h"
#include "../include/gfx/tiny_obj_loader.h"

class Triangle;

class Mesh {
	public:
		Mesh(tinyobj::attrib_t attrib, tinyobj::shape_t shape)
			: vertices(&attrib.vertices),
			  normals(&attrib.normals),
			  uvs(&attrib.texcoords),
			  indices(shape.mesh.indices),
			  ntris(shape.mesh.num_face_vertices.size()),
			  nverts(attrib.vertices.size()) {}

		const std::vector<tinyobj::real_t>* vertices;
		const std::vector<tinyobj::real_t>* normals;
		const std::vector<tinyobj::real_t>* uvs;
		std::vector<tinyobj::index_t> indices;

		size_t ntris;
		size_t nverts;
};