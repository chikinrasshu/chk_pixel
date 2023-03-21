#include "algebra.h"

float chk_map_range(float value, float a0, float a1, float b0, float b1)
{
    float slope = (b1 - b0) / (a1 - a0);
    return b0 + slope * (value - a0);
}