#include <cmath>

bool equalf(float A, float B, float maxRelDiff = 1e-8f)
{
    // Calculate the difference.
    float diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    // Find the largest
    float largest = (B > A) ? B : A;

    if (diff <= largest * maxRelDiff)
        return true;
    return false;
}

bool is_negative(float value, float eps = 1e-8f) {
    return value < -eps;
}

bool is_near_zero(float value, float eps = 1e-8f) {
    return std::fabs(value) < eps;
}