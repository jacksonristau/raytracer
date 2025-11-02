#include "../include/math/floatutil.h"
#include <cmath>
#include <iostream>

bool equalf(float a, float b)
{
    if (a == b)
        return true;
    // Calculate the difference.
    auto diff = std::abs(a-b);
    auto norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<float>::max());
    std::cout << "diff: " << diff << '\n';
    std::cout << "norm * eps: " << Eps * norm << '\n';
    std::cout << "Min: : " << Min << '\n';
    std::cout << "lowest: " << std::fmax(Min, Eps* norm)    << '\n';
    return diff < std::fmax(Min, Eps * norm);   
}   

bool is_negative(float value) {
    return value < -Eps;
}

bool is_near_zero(float value) {
    return std::fabs(value) < Eps;
}

bool is_greater_than_zero(float value) {
    return value > Eps;
}