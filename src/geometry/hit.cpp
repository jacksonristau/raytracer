#include "../include/geometry/hit.h"

Hit Hit::infinity() {
    return Hit(INFINITY, 0.0f, 0.0f, Ray(), Point3(), Vector3(), nullptr);
}