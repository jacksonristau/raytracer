#pragma once

#include "math/vector3.h"
#include "math/ray.h"
#include "color.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Camera {
	public:
		Camera();
		Camera(int res[], float hfov, Color dc, float alpha[2], float dist[2], Point3 e, Vector3 v, Vector3 u);
		Camera(json cam);
		~Camera();

		Ray generate_ray(int x, int y);

		Color depth_cue(Point3 x_p, Color i) const;

		inline int px_width() const { return resolution[0]; }
		inline int px_height() const { return resolution[1]; }

		Point3 eye_pos;
		Vector3 view_dir;
		Vector3 up_dir;
	private:

		Point3 ul;

		Vector3 deltah;
		Vector3 deltav;

		int resolution[2];

		// parallel projection
		float frustum_w = -1.0f;
		bool parallel = false;
		// radians
		float hfov;

		// depth cueing
		Color depth_color = Color(-1, -1, -1);
		float alpha[2];
		float dist[2];
};