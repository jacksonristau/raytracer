#pragma once
#include <memory>

class IShape;
class Hit;
class Material;
class Ray;

class Primitive {
	public:
		Primitive() : geometry(nullptr), material(nullptr), id(-1) {}
		Primitive(std::shared_ptr<IShape> geometry, std::shared_ptr<Material> material, int id)
			: geometry(std::move(geometry)), material(std::move(material)), id(id) {
		}
		~Primitive() {}

		Hit intersect(const Ray& r);


		std::shared_ptr<IShape> geometry;
		std::shared_ptr<Material> material;
		int id;

		bool operator==(const Primitive& other) const {
			return id == other.id;
		}
};