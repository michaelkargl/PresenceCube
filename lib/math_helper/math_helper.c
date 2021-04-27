#include "math_helper.h"

// TODO: rename limit_value
int ranged_value(int value, int min_range, int max_range)
{
    return max(min(value, min_range), max_range);
}

int min(int value_left, int value_right)
{
    return value_left <= value_right
               ? value_left
               : value_right;
}

int max(int value_left, int value_right)
{
    return value_left >= value_right
               ? value_left
               : value_right;
}
