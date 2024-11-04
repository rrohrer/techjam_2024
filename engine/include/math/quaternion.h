/**
 * @file
 * @author Ryan Rohrer <ryan.rohrer@gmail.com>
 *
 * @section DESCRIPTION
 * A Quaternion and its operations needed for 3D graphics.
 */
#ifndef Quaternion_h
#define Quaternion_h

#include "math/vector4.h"

/**
 * A Quaternion.
 *
 * x, y, z - the imaginary ijk elements.
 * w - the real element.
 */
struct Quaternion {
  float x;
  float y;
  float z;
  float w;
};

/**
 * Constructor for a Quaternion.
 *
 * @param x - (i) the first imaginary element.
 * @param y - (j) the second imaginary element.
 * @param z - (k) the third imaginary element.
 * @param w - (r) the real element.
 * @return - a newly constructed Quaternion.
 */
struct Quaternion Quaternion_new(float x, float y, float z, float w);

/**
 * Construct a Quaternion from an euler angle rotation.
 *
 * @param x - the rotation around X.
 * @param y - the rotation around Y.
 * @param z - the rotation around Z.
 * @return - a Quaternion that describes that rotation.
 */
struct Quaternion Quaternion_new_euler(float x, float y, float z);

/**
 * Construct a Quaternion from an axis/angle rotation.
 *
 * @param axis - the axis to rotate around.
 * @param angle - the angle to rotate around.
 * @return - the Quaternion that represents that particular axis angle rotation.
 */
struct Quaternion Quaternion_new_axis_angle(struct Vector4 axis, float angle);

/**
 * Create a new identity Quaternion.
 *
 * @return - a new Quaternion where its elements are 0,0,0,1.
 */
struct Quaternion Quaternion_identity();

/**
 * Convert a Quaternion to an axis/angle rotation.
 *
 * @param q - the Quaternion to convert.
 * @param axis - the axis it is rotating around.
 * @param angle -  the angle in radians that it is rotating.
 */
void Quaternion_to_axis_angle(struct Quaternion q, struct Vector4 *axis,
                              float *angle);

/**
 * Convert a Quaternion to euler angles.
 *
 * @param q - the Quaternion that will be converted to euler angles.
 * @return - a Vector4 containing the x,y,z rotations in radians.
 */
struct Vector4 Quaternion_to_euler(struct Quaternion q);

/**
 * Multiply two Quaternions together so that q = a * b.
 *
 * @param a - the first element to be multiplied.
 * @param b - the second element to be multiplied.
 * @return - the result of the Quaternion multiplication.
 */
struct Quaternion Quaternion_multiply(struct Quaternion a, struct Quaternion b);

/**
 * Transform a vector by the Quaternion q.
 *
 * @param q - the rotation to be applied to the Vector4 v.
 * @param v - the vector to be transformed.
 * @return - a Vector4 that has the transformation applied to it, with w
 *           left as passthrough.
 */
struct Vector4 Quaternion_transform(struct Quaternion q, struct Vector4 v);

/**
 * Inverse a Quaternion that is not normalized.
 *
 * @param q - the Quaternion to inverse.
 * @return the inverse of q.
 */
struct Quaternion Quaternion_inverse(struct Quaternion q);

/**
 * Inverse a unit (normalized) Quaternion.
 *
 * @param q - a normalized Quaternion.
 * @return - the inverse of q.
 */
struct Quaternion Quaternion_inverse_unit(struct Quaternion q);

/**
 * The length of the Quaternion.
 *
 * @param q - the Quaternion to find the length of.
 * @return - the length of q.
 */
float Quaternion_length(struct Quaternion q);

/**
 * Normalize a Quaternion.
 *
 * @param q - a Quaternion to be normalized.
 * @return - the normalized Quaternion, it will be unit length.
 */
struct Quaternion Quaternion_normalized(struct Quaternion q);

/**
 * Perform spherical linear interpolation.
 *
 * @param a - the start point of the slerp.
 * @param b - the end point of the slerp.
 * @param t - the amount to interpolate on.
 * @return - the Quaternion in between a and b by t.
 */
struct Quaternion Quaternion_slerp(struct Quaternion a, struct Quaternion b,
                                   float t);

/**
 * Dot product of two Quaternions.
 *
 * @param a - the first Quaternion to dot.
 * @param b - the second Quaternion to dot.
 * @return - the angle between the two Quaternions.
 */
float Quaternion_dot(struct Quaternion a, struct Quaternion b);

#endif
