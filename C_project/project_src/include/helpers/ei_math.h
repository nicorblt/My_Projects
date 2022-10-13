#ifndef EI_MATH_H
#define EI_MATH_H

/**
 * @brief Return the minimum of two values.
 *
 * @param a first value.
 * @param b second value.
 *
 * @return the minimum of a and b.
 *
 */
#define MAX(a, b) (((a) >= (b)) ? (a) : (b))

/**
 * @brief Return the maximum of two values.
 *
 * @param a first value.
 * @param b second value.
 *
 * @return the maximum of a and b.
 *
 */
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))

/**
 * @brief Return the absolute value of a value.
 *
 * @param a value.
 *
 * @return the absolute value of a.
 *
 */
#define ABS(a) (((a) < 0) ? -1 * (a) : (a))

#endif