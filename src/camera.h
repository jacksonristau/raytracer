#pragma once

#include "math/vector.h"
#include "math/ray.h"
#include "color.h"

class Camera {
	public:
		Camera();
		Camera(int res[], int hfov, Color dc, float alpha[2], float dist[2], Vector e, Vector v, Vector u);
		~Camera();

		Ray generate_ray(int x, int y);

		Color depth_cue(Vector x, Color i) const;

		Vector pos() const { return eye_pos; }
		Vector view() const { return view_dir; }
		Vector up() const { return up_dir; }

		float fov() const { return hfov; }
		float frustum_width() const { return frustum_w; }
		bool is_parallel() const { return parallel; }

		inline int px_width() const { return resolution[0]; }
		inline int px_height() const { return resolution[1]; }
	private:
		Vector eye_pos;
		Vector view_dir;
		Vector up_dir;

		Vector ul;

		Vector deltah;
		Vector deltav;

		int resolution[2];

		// parallel projection
		float frustum_w = -1.0f;
		bool parallel = false;
		float hfov;

		// depth cueing
		Color depth_color = Color(-1, -1, -1);
		float alpha[2];
		float dist[2];
};