#pragma once

#include <vector>
#include "../include/math/vector3.h"
#include "../include/math/point2.h"
#include "../include/gfx/tiny_obj_loader.h"

class Triangle;

class Mesh {
	public:
		Mesh(std::shared_ptr<tinyobj::attrib_t> attrib, tinyobj::shape_t shape)
			: attrib(attrib),
			  indices(shape.mesh.indices),
			  ntris(shape.mesh.num_face_vertices.size()),
			  nverts(attrib->vertices.size()) {}

		std::vector<tinyobj::real_t> get_vertices() const {return attrib->vertices;}
		std::vector<tinyobj::real_t> get_normals() const {return attrib->normals;}
		std::vector<tinyobj::real_t> get_uvs() const {return attrib->texcoords;}

		const std::shared_ptr<tinyobj::attrib_t> attrib;
		std::vector<tinyobj::index_t> indices;

		size_t ntris;
		size_t nverts;
};