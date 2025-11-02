#pragma once


class IShape;
class Hit;
class Material;
class Ray;

class Primitive {
	public:
		Primitive();
		~Primitive();

		Hit intersect(const Ray& r);


		IShape* geometry;
		Material* material;
		int id;

		bool operator==(const Primitive& other) const {
			return id == other.id;
		}
};