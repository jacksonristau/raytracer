#pragma once
#include <memory>
#include "../math/transform.h"

class IShape;
class Hit;
class Material;
class Ray;
class AABB;

class Primitive {
	public:
		Primitive() : geometry(nullptr), material(nullptr), id(-1), transform() {}
		Primitive(std::shared_ptr<IShape> geometry, std::shared_ptr<Material> material, int id, const Transform& transform = Transform())
			: geometry(std::move(geometry)), material(std::move(material)), id(id), transform(transform) {
		}
		~Primitive() {}

		Hit intersect(const Ray& r) const;
		AABB get_aabb() const;


		std::shared_ptr<IShape> geometry;
		std::shared_ptr<Material> material;
		int id;
		Transform transform;

		bool operator==(const Primitive& other) const {
			return id == other.id;
		}
};