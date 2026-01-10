#include "aabb.h"
#include "primitive.h"

struct BVHNode {
	AABB bounds;
	uint32_t start;
	uint32_t end;
	uint32_t left;
	uint32_t right;
		
	static BVHNode branch(AABB bounds, uint32_t left, uint32_t right) {
		BVHNode node = BVHNode();
		node.bounds = bounds;
		node.left = left;
		node.right = right;
		node.start = 0;
		node.end = 0;
		return node;
	}

	static BVHNode leaf(AABB bounds, uint32_t start, uint32_t end) {
		BVHNode node = BVHNode();
		node.bounds = bounds;
		node.left = 0;
		node.right = 0;
		node.start = start;
		node.end = end;
		return node;
	}

	bool is_leaf() const { return end > 0;  }
};

class BVH {
	BVH(std::vector<Primitive*> primitives);
	uint32_t build_bvh_object_median(uint32_t& node_index, uint32_t start_index, uint32_t end_index);

	std::vector<Primitive*> primitives;
	std::vector<BVHNode> nodes;
};