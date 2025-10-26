#pragma once
#include "../math/floatutil.h"

struct Hit {
    float t = -1.0f;    
    int prim_index = -1; 

    float u = 0.0f;
    float v = 0.0f;

    bool valid() const { return ! is_negative(t); }
};