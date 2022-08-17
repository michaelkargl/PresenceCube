#include "math_util.h"
#include "CException.h"


// TODO: rename limit_value
int ranged_value(int value, int min_range, int max_range)
{
    if(min_range > max_range) {
        Throw(ERROR_CODE_LOWER_BOUNDS_BIGGER_THAN_UPPER_BOUNDS);
    }

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
