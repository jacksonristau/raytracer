#include "camera.h"

Camera::Camera() {
	eye_pos, view_dir, up_dir = Vector();
    resolution[0] = 0;
    resolution[1] = 0;
    hfov = 45;
    dist[0] = 0;
    dist[1] = 0;
    alpha[0] = 0;
    alpha[1] = 0;
}

Camera::Camera(int res[], float fov, Color dc, float a[2], float d[2], Vector eye, Vector view, Vector up) {
    resolution[0] = res[0];
    resolution[1] = res[1];
    depth_color = dc;
    alpha[0] = a[0];
    alpha[1] = a[1];
    dist[0] = d[0];
    dist[1] = d[1];
	eye_pos = eye;
	view_dir = view;
	up_dir = up;
    hfov = fov;

    // define the viewing coordinate system
    Vector u = view_dir.cross(up_dir);
    u.normalize();
    Vector v = u.cross(view_dir);

    float aspect = (float)resolution[0] / (float)resolution[1];
    float d_val = 2;

    // width is just frustum width for a parallel projection
    float width = parallel ? frustum_w : 2 * d_val * tan(hfov / 2);
    float height = width / aspect;
    std::cout << "camera resolution: " << resolution[0] << " x " << resolution[1] << "\n";
    // go to view plane then to the left/right edge, then to the top/bottom
    ul = (eye_pos + d_val * view_dir) - ((width / 2.0f) * u) + ((height / 2.0f) * v);
    Vector ur = (eye_pos + d_val * view_dir) + ((width / 2.0f) * u) + ((height / 2.0f) * v);

    Vector ll = (eye_pos + d_val * view_dir) - ((width / 2.0f) * u) - ((height / 2.0f) * v);
    Vector lr = (eye_pos + d_val * view_dir) + ((width / 2.0f) * u) - ((height / 2.0f) * v);

    deltah = (1.0f / (resolution[0] - 1)) * (ur - ul);
    deltav = (1.0f / (resolution[1] - 1)) * (ll - ul);
}

Camera::~Camera()
{
}

Ray Camera::generate_ray(int x, int y) {
    // parallel projection moves the origin around
    float xf = static_cast<float>(x);
    float yf = static_cast<float>(y);
	if (parallel) {
        Vector origin = ul + (xf * deltah) + (yf * deltav);
		return Ray(origin, view_dir);
	}
    // perspective projection moves the view direction around
	else {
        Vector direction = (ul + (yf * deltav) + (xf * deltah)) - eye_pos;
		return Ray(eye_pos, direction);
	}
}

Color Camera::depth_cue(Vector x, Color i) const {
    Color final_color = i;
    if (depth_color != Color(-1, -1, -1)) {
        float view_dist = eye_pos.distance(x);
        float alpha_dc;
        if (view_dist <= dist[0]) {
            alpha_dc = alpha[1];
        }
        else if (view_dist >= dist[1]) {
            alpha_dc = alpha[0];
        }
        else {
            alpha_dc = alpha[0] + (alpha[1] - alpha[0]) * ((dist[1] - view_dist) / (dist[1] - dist[0]));
        }
        final_color = (alpha_dc * i + (1 - alpha_dc) * depth_color);
    }
    return final_color;
}