#include "../include/geometry/bvh.h"
#include "../include/geometry/aabb.h"
#include "../include/geometry/primitive.h"
#include "../include/geometry/shape.h"
#include "../include/gfx/material.h"
#include <vector>
#include <gfx/lights.h>
BVH::BVH(){
	primitives = std::vector<Primitive>();
	nodes = std::vector<BVHNode>();
}
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

	uint32_t left = build_bvh_object_median(next_node_index, start_index, mid_index);
	uint32_t right = build_bvh_object_median(next_node_index, mid_index, end_index);
	
	nodes[node_index] = BVHNode::branch(bounds, left, right);
	return node_index;
}

Hit BVH::intersect_node(const BVHNode& node, const Ray& ray) const {
	Hit closest = Hit::infinity();
	for (uint32_t i = node.start; i < node.end; i++) {
		const Primitive& p = primitives[i];
		if (ray.o_primitive != nullptr && p == *ray.o_primitive)
			continue;
		Hit h = p.intersect(ray);
		if (h.valid() && h.t < closest.t) {
			closest = h;
		}
	}
	return closest;
}

float BVH::intersect_node_shadow(const BVHNode& node, const Ray& ray, const ILight* l, float d) const {
	float s = 1.0f;
	for (uint32_t i = node.start; i < node.end; i++) {
		const Primitive& p = primitives[i];
		if (ray.o_primitive != nullptr && p == *ray.o_primitive)
			continue;
		Hit h = p.intersect(ray);
		if (!h.valid() || is_negative(h.t))
			continue;
		if (l->is_point() && h.t > d)
			continue;
		s *= (1 - p.material->alpha());
		if (is_near_zero(s))
			return s;
	}
	return s;
}
float BVH::traverse_shadow(const BVHNode& node, const Ray& ray, const ILight* l, float d) const {
	float t_bounds = node.bounds.intersect(ray);
	if (is_negative(t_bounds))
		return 1.0f;

	if (node.is_leaf()) {
		float s = intersect_node_shadow(node, ray, l, d);
		return s;
	}
	else {
		float left = traverse_shadow(nodes[node.left], ray, l, d);
		if (is_near_zero(left))
			return 0.0f;
		else 
		return left * traverse_shadow(nodes[node.right], ray, l, d);
	}
}
// intersect with root 
// if closer than t min process the bvhnode
// process the closest node first
// if inner nodes recurse. further ones on the function stack if need be
// if its a leaf check all the triangles and update t min for closest triangle intersection
void BVH::traverse(const BVHNode& node, const Ray& ray, Hit& closest) const {
	float t_bounds = node.bounds.intersect(ray);
	if (is_negative(t_bounds) || t_bounds > closest.t)
		return;

	if (node.is_leaf()) {
		Hit h = intersect_node(node, ray);
		if (h.valid() && h.t < closest.t)
			closest = h;
	}
	else {
		const BVHNode& left_node = nodes[node.left];
		const BVHNode& right_node = nodes[node.right];

		const float left_t = left_node.bounds.intersect(ray);
		const float right_t = right_node.bounds.intersect(ray);
		
		const bool left_valid = (!is_negative(left_t) && left_t < closest.t);
		const bool right_valid = (!is_negative(right_t) && right_t < closest.t);

		if (left_valid && right_valid) {
			if (left_t < right_t) {
				traverse(nodes[node.left], ray, closest);
				traverse(nodes[node.right], ray, closest);
			}
			else {
				traverse(nodes[node.right], ray, closest);
				traverse(nodes[node.left], ray, closest);
			}
		}
		else if (left_valid) {
			traverse(nodes[node.left], ray, closest);
		}
		else if (right_valid) {
			traverse(nodes[node.right], ray, closest);
		}
	}
}