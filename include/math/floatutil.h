#include <cmath>
#include "../include/math/constants.h"

bool equalf(float A, float B, float maxRelDiff = Eps)
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

bool is_negative(float value, float eps = Eps) {
    return value < -eps;
}

bool is_near_zero(float value, float eps = Eps) {
    return std::fabs(value) < eps;
}

bool is_greater_than_zero(float value, float eps = Eps) {
    return value > eps;
}