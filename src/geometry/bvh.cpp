#include "../include/geometry/bvh.h"
#include "../include/geometry/aabb.h"
#include "../include/geometry/primitive.h"
#include "../include/geometry/shape.h"
#include <vector>

BVH::BVH(std::vector<Primitive> primitives) : primitives(primitives){
	nodes = std::vector<BVHNode>((2 * primitives.size()) - 1);
}

uint32_t BVH::build_bvh_object_median(uint32_t& next_node_index, uint32_t start_index, uint32_t end_index) {
	uint32_t node_index = next_node_index++;
	auto start_iter = primitives.begin() + start_index;
	auto end_iter = primitives.begin() + end_index;

	AABB bounds = AABB(start_iter, end_iter);

	if (end_index - start_index <= 4) {
		nodes[node_index] = BVHNode::leaf(bounds, start_index, end_index);
		return node_index;
	}

	// object median
	uint32_t mid_index = start_index + (end_index - start_index) / 2;
	auto mid_iter = primitives.begin() + mid_index;

	Vector3 extent = bounds.max - bounds.min;
	int axis = 0;
	if (extent.y > extent.x) axis = 1;
	if (extent.z > extent[axis]) axis = 2;
	auto cmp = [axis](Primitive a, Primitive b) {
		return a.geometry->get_centroid()[axis] < b.geometry->get_centroid()[axis];
	};
	std::nth_element(start_iter, mid_iter, end_iter, cmp);

	uint32_t left = build_bvh_object_median(node_index, start_index, mid_index);
	uint32_t right = build_bvh_object_median(node_index, mid_index, end_index);
	
	nodes[node_index] = BVHNode::branch(bounds, left, right);
	return node_index;
}

void BVH::traverse(BVHNode node, Ray& ray, int& t) {
	node.bounds.

	// intersect with root 
	// if closer than t min process the bvhnode
	// process the closest node first
	// if inner nodes recurse. further ones on the function stack if need be
	// if its a leaf check all the triangles and update t min for closest triangle intersection
}