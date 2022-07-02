#include "math_util.h"

// TODO: rename limit_value
int ranged_value(int value, int min_range, int max_range)
{
    char hits_lower_threshold = value <= min_range;
    if (hits_lower_threshold) {
        return min_range;
    }

    char hits_upper_threshold = value >= max_range;
    if (hits_upper_threshold) {
        return max_range;
    }

    return value;
}

int max(int value, int value2)
{
    return value >= value2
               ? value
               : value2;
}

int min(int value, int value2)
{
    return value <= value2
               ? value
               : value2;
}
