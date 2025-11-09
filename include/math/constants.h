#pragma once

#define Pi 3.14159265358979323846f
#define InvPi 0.31830988618379067154f
#define Inv2Pi 0.15915494309189533577f
#define Inv4Pi 0.07957747154594766788f
#define PiOver2 1.57079632679489661923f
#define PiOver4 0.78539816339744830961f
#define Sqrt2 1.41421356237309504880f
#define TwoPi 6.28318530718f
#if defined(_MSC_VER)
    #define Eps (128 * FLT_EPSILON)
    #define Min FLT_MIN
#elif defined(__GNUC__) || defined(__clang__)
    #define Eps (128 * __FLT_EPSILON__)
    #define Min __FLT_MIN__
#else
    #define Eps (128 * 1.19209290e-7F)
    #define Min 1.17549435e-38F
#endif