#pragma once

#include <vector>
#include "../include/math/vector3.h"
#include "../include/math/point2.h"

class Triangle;
class Mesh {
public:
	Mesh(
		std::vector<Point3> vertices,
		std::vector<Vector3> normals,
		std::vector<Point2> uvs,
		std::vector<int> vertex_indices,
		std::vector<int> texture_indices,
		std::vector<int> normal_indices
	) :vertices(vertices),
		normals(normals),
		uvs(uvs),
		vertex_indices(vertex_indices),
		texture_indices(texture_indices),
		normal_indices(normal_indices),
		ntris(vertex_indices.size() / 3),
		nverts(vertices.size()) {
	}

	std::vector<Point3> vertices;
	std::vector<Vector3> normals;
	std::vector<Point2> uvs;
	std::vector<int> vertex_indices;
	std::vector<int> texture_indices;
	std::vector<int> normal_indices;

	int ntris;
	int nverts;
};