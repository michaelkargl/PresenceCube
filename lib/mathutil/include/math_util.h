#pragma once

/**
 * @brief Determines the smaller of 2 given numbers
 * 
 * @param value input number 1
 * @param min input number 2
 * @return int the smaller of the 2 input values
 */
int min(int value, int min);


/**
 * @brief Determines the bigger of 2 given numbers
 * 
 * @param value input number 1
 * @param max input number 2
 * @return int the bigger of the 2 input values
 */
int max(int value, int max);


/**
 * @brief Limits a value to configured set range: [lower_bound;upper_bound]
 * 
 * @pre ranged_value(0, 6, 10) => 6
 * @pre ranged_value(11, 6, 10) => 10
 * @pre ranged_value(6, 6, 10) => 6
 * @pre ranged_value(8, 6, 10) => 8
 * 
 * @param value The value to be limited
 * @param min_range lower bounds inclusive
 * @param max_range upper bounds inclusive
 * @return int A value that resides within the specified range
 */
int ranged_value(int value, int min_range, int max_range);
