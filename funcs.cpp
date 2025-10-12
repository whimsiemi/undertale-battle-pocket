#include <raylib.h>
#include "headers/funcs.hpp"

namespace funcs {
    // Custom random float function because Raylib doesn't have one
    float GetRandomFloat(float min, float max)
    {
        // Creates a random float between 0.0f and 1.0f by dividing a random int between 1 and 10000 by 10000f
        float random = static_cast<float>(GetRandomValue(0, 10000)) / 10000.0f;

        // Multiplies by random range to get relative random value within range
        float range = max - min;
        return (random * range) + min;
    }
}