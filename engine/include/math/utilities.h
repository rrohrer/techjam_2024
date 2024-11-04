/**
 * @file
 * @author Ryan Rohrer <ryan.rohrer@gmail.com>
 *
 * @section DESCRIPTION
 * Utilities to help with math and floating point things.
 */
#ifndef MathUtilities_h
#define MathUtilities_h

#include <math.h>

/**
 * Value used for most floating point equality comparisons.
 */
#define EPSILON 0.000000001f

/**
 * Value used for floating point comparisons that are expected to have a lot of
 * error.
 */
#define LOOSE_EPSILON 0.01f

/**
 * Compare two floats.
 *
 * @param x - first float to be compared.
 * @param y - the second float to be compared.
 * @return - true/false if the floats are close to equal.
 */
#define EPSILON_COMPARE(x, y) (fabs(x - y) < EPSILON)

/**
 * Compare two floats that are expected to be kinda far apart..
 *
 * @param x - first float to be compared.
 * @param y - the second float to be compared.
 * @return - true/false if the floats are close to equal.
 */
#define LOOSE_EPSILON_COMPARE(x, y) (fabs(x - y) < LOOSE_EPSILON)

#endif
