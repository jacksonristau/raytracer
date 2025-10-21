#pragma once

#include "math/vector3.h"
#include "math/ray.h"
#include "color.h"

class Camera {
	public:
		Camera();
		Camera(int res[], float hfov, Color dc, float alpha[2], float dist[2], Point3 e, Vector3 v, Vector3 u);
		~Camera();

		Ray generate_ray(int x, int y);

		Color depth_cue(Point3 x_p, Color i) const;

		Point3 pos() const { return eye_pos; }
		Vector3 view() const { return view_dir; }
		Vector3 up() const { return up_dir; }

		float fov() const { return hfov; }
		float frustum_width() const { return frustum_w; }
		bool is_parallel() const { return parallel; }

		inline int px_width() const { return resolution[0]; }
		inline int px_height() const { return resolution[1]; }
	private:
		Point3 eye_pos;
		Vector3 view_dir;
		Vector3 up_dir;

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