#pragma once


class IShape;
class Hit;
class Material;
class Ray;

class Primitive {
	public:
		Primitive() : geometry(nullptr), material(nullptr), id(-1) {}
		Primitive(IShape* geometry, Material* material) : geometry(geometry), material(material) {}
		~Primitive() {}

		Hit intersect(const Ray& r);


		IShape* geometry;
		Material* material;
		int id;

		bool operator==(const Primitive& other) const {
			return id == other.id;
		}
};