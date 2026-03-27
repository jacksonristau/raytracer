#pragma once

#include <vector>
#include "../include/math/vector3.h"
#include "../include/math/point2.h"
#include "../include/gfx/tiny_obj_loader.h"
#include "../include/math/matrix4x4.h"

class Triangle;

//class Mesh {
//	public:
//		Mesh(std::shared_ptr<tinyobj::attrib_t> attrib, tinyobj::shape_t shape)
//			: attrib(attrib),
//			  indices(shape.mesh.indices),
//			  ntris(shape.mesh.num_face_vertices.size()),
//			  nverts(attrib->vertices.size()) {}
//
//		const std::vector<tinyobj::real_t>& get_vertices() const {return attrib->vertices;}
//		const std::vector<tinyobj::real_t>& get_normals() const {return attrib->normals;}
//		const std::vector<tinyobj::real_t>& get_uvs() const {return attrib->texcoords;}
//
//		const std::shared_ptr<tinyobj::attrib_t> attrib;
//		std::vector<tinyobj::index_t> indices;
//
//		size_t ntris;
//		size_t nverts;
//};

class Mesh {
	public:
		Mesh(std::shared_ptr<tinyobj::attrib_t> attrib, tinyobj::shape_t shape)
			: attrib(attrib),
				indices(shape.mesh.indices),
				ntris(shape.mesh.num_face_vertices.size()),
				nverts(attrib->vertices.size()) {}
		void build(const Matrix4x4& obj_to_world);
		const std::vector<tinyobj::real_t>& get_vertices() const {return attrib->vertices;}
		const std::vector<tinyobj::real_t>& get_normals() const {return attrib->normals;}
		const std::vector<tinyobj::real_t>& get_uvs() const {return attrib->texcoords;}

		std::shared_ptr<tinyobj::attrib_t> attrib;
		std::vector<tinyobj::index_t> indices;
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> uvs;

		bool has_normals = false;
		bool has_uvs = false;
		size_t ntris;
		size_t nverts;
};