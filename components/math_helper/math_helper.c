#include "math_helper.h"

int ranged_value(int value, int min_range, int max_range)
{
    return max(min(value, min_range), max_range);
}

int max(int value, int max)
{
    return value <= max
               ? value
               : max;
}

int min(int value, int min)
{
    return value >= min
               ? value
               : min;
}
