/**
 * @file
 * @author Ryan Rohrer <ryan.rohrer@gmail.com>
 *
 * @section DESCRIPTION
 * A four float vector type.
 */
#ifndef Vector4_h
#define Vector4_h

/**
 * A 4D vector.
 */
struct Vector4 {
  float x;
  float y;
  float z;
  float w;
};

/**
 * Constructor for a 4D point (x, y, z, 1)
 *
 * @param x - the x coordinate.
 * @param y - the y coordinate.
 * @param z - the z coordinate.
 * @return - the newly constructed point.
 */
struct Vector4 Vector4_new_point(float x, float y, float z);

/**
 * Constructor for a 4D vector (x, y, z, 0)
 *
 * @param x - the x coordinate.
 * @param y - the y coordinate.
 * @param z - the z coordinate.
 * @return - the newly constructed vector.
 */
struct Vector4 Vector4_new_vector(float x, float y, float z);

/**
 * Creates a point at the origin of R3.
 *
 * @return - the point (0, 0, 0, 1)
 */
struct Vector4 Vector4_zero_point(void);

/**
 * Creates the zero vector for R3.
 *
 * @return - the vector (0, 0, 0, 0)
 */
struct Vector4 Vector4_zero_vector(void);

/**
 * Creates the unit point of R3.
 *
 * @return - the point (1, 1, 1, 1)
 */
struct Vector4 Vector4_one_point(void);

/**
 * Creates the unit vector of R3.
 *
 * @return - the vector (0, 0, 0, 1)
 */
struct Vector4 Vector4_one_vector(void);

/**
 * Calculates the magnitude of a Vector4.
 *
 * @param v - the Vector4 to be calculated.
 * @return - the magnitude of the Vector4.
 */
float Vector4_magnitude(struct Vector4 v);

/**
 * Calculates the magnitude^2 of a Vector4 for perf critical things.
 *
 * @param v - the Vector4 to be calculated.
 * @return - the magnitude squared of the Vector4.
 */
float Vector4_magnitude_squared(struct Vector4 v);

/**
 * Creates a normalized copy of the Vector4.
 *
 * @param v - the Vector4 that will be normalized.
 * @return - a normalized copy of the Vector4.
 */
struct Vector4 Vector4_normalized(struct Vector4 v);

/**
 * Normalizes a Vector4 in place.
 *
 * @param v - a pointer to the Vector4 that will be normalized.
 */
void Vector4_normalize(struct Vector4 *v);

/**
 * Calculates the distance between two Vector4s.
 *
 * @param a - the first Vector4 in the calculation.
 * @param b - the second Vector4 in the calculation.
 * @return - the distance between these two Vector4s.
 */
float Vector4_distance(struct Vector4 a, struct Vector4 b);

/**
 * Calculates the distance squared between two Vector4s as a
 * performance optimization.
 *
 * @param a - the first Vector4 in the calculation.
 * @param b - the second Vector4 in the calculation.
 * @return - the distance squared between the two vectors.
 */
float Vector4_distance_squared(struct Vector4 a, struct Vector4 b);

/**
 * Calculates the dot product of two Vector4s.
 *
 * @param a - the first Vector4 to be dotted.
 * @param b - the second Vector4 to be dotted.
 * @return - the result of a dot b.
 */
float Vector4_dot(struct Vector4 a, struct Vector4 b);

/**
 * Calculates the cross product of two Vector4s.
 *
 * @param a - the first Vector4 to be crossed.
 * @param b - the second Vector4 to be crossed.
 * @return - the result of a (X) b.
 */
struct Vector4 Vector4_cross(struct Vector4 a, struct Vector4 b);

/**
 * Lerps a Vector4 between two vectors.
 *
 * @param a - the Vector4 that starts the lerp.
 * @param b - the Vector4 that ends the lerp.
 * @param t - the interpolation value for a -> b.
 * @return - the liniarly interpolated Vector4 from a->b based off t.
 */
struct Vector4 Vector4_lerp(struct Vector4 a, struct Vector4 b, float t);

/**
 * Adds two Vector4s.
 *
 * @param a - the first Vector4 to add.
 * @param b - the second Vector4 to add.
 * @return - the result of a + b.
 */
struct Vector4 Vector4_add(struct Vector4 a, struct Vector4 b);

/**
 * Subtracts two Vector4s.
 *
 * @param a - the Vector4 to be subtracted from.
 * @param b - the Vector4 subtracted.
 * @return - the result of a - b.
 */
struct Vector4 Vector4_subtract(struct Vector4 a, struct Vector4 b);

/**
 * Scales a vecfor4 by a float.
 *
 * @param v - the Vector4 to be scaled.
 * @param s - the scalar to apply to v.
 * @return - the result of s * v;
 */
struct Vector4 Vector4_scale(struct Vector4 v, float s);

#endif
