#pragma once

/**
 * @brief Determines the smaller of 2 given numbers
 * 
 * @param value input number 1
 * @param math_util__min input number 2
 * @return int the smaller of the 2 input values
 */
int math_util__min(int value, int min);


/**
 * @brief Determines the bigger of 2 given numbers
 * 
 * @param value input number 1
 * @param max input number 2
 * @return int the bigger of the 2 input values
 */
int math_util__max(int value, int max);


/**
 * @brief Limits a value to configured set range: [lower_bound;upper_bound]
 * 
 * @pre math_util__clamp(0, 6, 10) => 6
 * @pre math_util__clamp(11, 6, 10) => 10
 * @pre math_util__clamp(6, 6, 10) => 6
 * @pre math_util__clamp(8, 6, 10) => 8
 * 
 * @param value The value to be limited
 * @param min_range lower bounds inclusive
 * @param max_range upper bounds inclusive
 * @return int A value that resides within the specified range
 */
int math_util__clamp(int value, int min_range, int max_range);
