#include <raylib.h>

#ifndef FUNCS_HPP
#define FUNCS_HPP

namespace funcs {
    // Custom random float function because Raylib doesn't have one
    float GetRandomFloat(float min, float max);
}

#endif