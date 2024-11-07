/**
 * @file
 * @author Ryan Rohrer <ryan.rohrer@gmail.com>
 *
 * @section DESCRIPTION
 * A 4x4 square matrix that works with OpenGL.
 */
#ifndef Matrix4_h
#define Matrix4_h

#include "math/vector4.h"

/**
 * A 4x4 square matrix.
 *
 * Column major. Members are named f[row][column].
 * All can be accessed as an array with f[i].
 *
 * Member layout:
 * f11 f12 f13 f14
 * f21 f22 f23 f24
 * f31 f32 f33 f34
 * f41 f42 f43 f44
 *
 * Index Layout:
 * 00 04 08 12
 * 01 05 09 13
 * 02 06 10 14
 * 03 07 11 15
 */
struct Matrix4 {
  union {
    struct {
      float f11, f12, f13, f14, f21, f22, f23, f24, f31, f32, f33, f34, f41,
          f42, f43, f44;
    };
    float f[16];
  };
};

/**
 * Computes the determinant of a Matrix4.
 *
 * @param m - the Matrix4 that will be calculated.
 * @return - the determinant of the matrix.
 */
float Matrix4_determinant(struct Matrix4 const *m);

/**
 * Generically calculate a frustum. Same as gluFrustum.
 *
 * @param left - the left bound of the frustum.
 * @param right - thr right bound of the frustum.
 * @param bottom - the lower bound of the frustum.
 * @param top - the upper bound of the frustum.
 * @param near - the near plane of the frustum.
 * @param far - the far plane of the frustum.
 * @return - a Matrix4 that represents this projection frustum.
 */
struct Matrix4 Matrix4_frustum(float left, float right, float bottom, float top,
                               float near, float far);

/**
 * Returns the first column as a Vector4.
 *
 * @param m - the matrix from which to return a column.
 * @return - Vector4 (f11, f21, f31, f41).
 */
struct Vector4 Matrix4_get_column1(struct Matrix4 const *m);

/**
 * Returns the second column as a Vector4.
 *
 * @param m - the matrix from which to return the column.
 * @return - Vector4 (f12, f22, f32, f42).
 */
struct Vector4 Matrix4_get_column2(struct Matrix4 const *m);

/**
 * Returns the third column as a Vector4.
 *
 * @param m - the matrix from which to return the column.
 * @return - Vector4 (f13, f23, f33, f43).
 */
struct Vector4 Matrix4_get_column3(struct Matrix4 const *m);

/**
 * Returns the fourth column as a Vector4.
 *
 * @param m - the matrix from which to return the column.
 * @return - Vector4 (f14, f24, f34, f44).
 */
struct Vector4 Matrix4_get_column4(struct Matrix4 const *m);

/**
 * Returns the first row as a Vector4.
 *
 * @param m - the matrix from which to return the column.
 * @return - Vector4 (f11, f12, f13, f14).
 */
struct Vector4 Matrix4_get_row1(struct Matrix4 const *m);

/**
 * Returns the second row as a Vector4.
 *
 * @param m - the matrix from which to return the row.
 * @return - Vector4 (f21, f22, f23, f24).
 */
struct Vector4 Matrix4_get_row2(struct Matrix4 const *m);

/**
 * Returns the third row as a Vector4.
 *
 * @param m - the matrix from which to return the row.
 * @return - Vector4 (f31, f32, f33, f34).
 */
struct Vector4 Matrix4_get_row3(struct Matrix4 const *m);

/**
 * Returns the fourth row as a Vector4.
 *
 * @param m - the matrix from which to return the row.
 * @return - Vector4 (f41, f42, f43, f44).
 */
struct Vector4 Matrix4_get_row4(struct Matrix4 const *m);

/**
 * Returns a new identity matrix.
 *
 * @return - a Matrix4 that has the diagonal set to 1.
 */
struct Matrix4 Matrix4_identity(void);

/**
 * Inverts a Matrix4.
 *
 * @param m - the Matrix4 that will be inverted (if possible).
 * @return - m^-1 if it exists, otherwise the identity matrix.
 */
struct Matrix4 Matrix4_invert(struct Matrix4 const *m);

/**
 * Generate a camera matrix that is looking at a target. RH
 *
 * @param eye - the location of the camera.
 * @param target - the target that the camera is focusing on.
 * @param up - a vector orthogonal to (eye - target) that represents up.
 * @return - a camera matrix focusing on target.
 */
struct Matrix4 Matrix4_lookat(struct Vector4 eye, struct Vector4 target,
                              struct Vector4 up);

/**
 * Multiply two Matrix4's together. A * B
 *
 * @param a - the first matrix to multiply.
 * @param b - the second matrix to multiply.
 * @return - the result of a * b.
 */
struct Matrix4 Matrix4_multiply(struct Matrix4 const *a,
                                struct Matrix4 const *b);

/**
 * Generates an orthographic projection matrix.
 *
 * @param left - the left bound of the projection.
 * @param right - the right bound of the projection.
 * @param bottom - the bottom bound of the projection.
 * @param top - the top bound of the projection.
 * @return - an orthographic projection between -1 and 1.
 */
struct Matrix4 Matrix4_orthographic(float left, float right, float bottom,
                                    float top);

/**
 * Generates an orthographic projection matrix.
 *
 * @param left - the left bound of the projection.
 * @param right - the right bound of the projection.
 * @param bottom - the bottom bound of the projection.
 * @param top - the top bound of the projection.
 * @param near - the near bound of the projection.
 * @param far - the far bound of the projection.
 * @return - an orthographic projection.
 */
struct Matrix4 Matrix4_orthographic_nf(float left, float right, float bottom,
                                       float top, float near, float far);

/**
 * Generates a perspective projection matrix. Behaves like gluPerspective (but
 * with radians). RH
 *
 * @param fovy - the vertical field of view of this projection.
 * @param aspect_ratio - the width:height ration of the projection.
 * @param near - the near plane of the projection.
 * @param far - the far plane of the projection.
 * @return - a perspective projection matrix.
 */
struct Matrix4 Matrix4_perspective(float fovy, float aspect_ratio, float near,
                                   float far);

/**
 * Generates a rotation matrix around the X axis.
 *
 * @param angle - the angle to rotate in radians.
 * @return - the Matrix4 that represents the rotation.
 */
struct Matrix4 Matrix4_rotation_x(float angle);

/**
 * Generates a rotation matrix around the Y axis.
 *
 * @param angle - the angle to rotate in radians.
 * @return - the Matrix4 that represents the rotation.
 */
struct Matrix4 Matrix4_rotation_y(float angle);

/**
 * Generates a rotation matrix around the Z axis.
 *
 * @param angle - the angle to rotate in radians.
 * @return - the Matrix4 that represents the rotation.
 */
struct Matrix4 Matrix4_rotation_z(float angle);

/**
 * Sets the first column of the given matrix.
 *
 * @param m - the matrix to be set.
 * @param v - the vector to assign.
 */
void Matrix4_set_column1(struct Matrix4 *m, struct Vector4 v);

/**
 * Sets the second column of the given matrix.
 *
 * @param m - the matrix to be set.
 * @param v - the vector to assign.
 */
void Matrix4_set_column2(struct Matrix4 *m, struct Vector4 v);

/**
 * Sets the third column of the given matrix.
 *
 * @param m - the matrix to be set.
 * @param v - the vector to assign.
 */
void Matrix4_set_column3(struct Matrix4 *m, struct Vector4 v);

/**
 * Sets the fourth column of the given matrix.
 *
 * @param m - the matrix to be set.
 * @param v - the vector to assign.
 */
void Matrix4_set_column4(struct Matrix4 *m, struct Vector4 v);

/**
 * Sets the first row of the given matrix.
 *
 * @param m - the matrix to be set.
 * @param v - the vector to assign.
 */
void Matrix4_set_row1(struct Matrix4 *m, struct Vector4 v);

/**
 * Sets the second row of the given matrix.
 *
 * @param m - the matrix to be set.
 * @param v - the vector to assign.
 */
void Matrix4_set_row2(struct Matrix4 *m, struct Vector4 v);

/**
 * Sets the third row of the given matrix.
 *
 * @param m - the matrix to be set.
 * @param v - the vector to assign.
 */
void Matrix4_set_row3(struct Matrix4 *m, struct Vector4 v);

/**
 * Sets the fourth row of the given matrix.
 *
 * @param m - the matrix to be set.
 * @param v - the vector to assign.
 */
void Matrix4_set_row4(struct Matrix4 *m, struct Vector4 v);

/**
 * Generates a scale matrix.
 *
 * @param x - the scale along the x axis.
 * @param y - the scale along the y axis.
 * @param z - the scale along the z axis.
 * @return - a scale Matrix4.
 */
struct Matrix4 Matrix4_scale(float x, float y, float z);

/**
 * Multiply a Matrix4 and a Vector4. M * V
 *
 * @param m - the matrix that will transform the vector.
 * @param v - the vector being transformed.
 * @return - the result of m*v.
 */
struct Vector4 Matrix4_transform(struct Matrix4 const *m, struct Vector4 v);

/**
 * Generates a translation matrix.
 *
 * @param x - the translation along the x axis.
 * @param y - the translation along the y axis.
 * @param z - the translation along the z axis.
 * @return - a matrix that represents this translation.
 */
struct Matrix4 Matrix4_translation(float x, float y, float z);

/**
 * Transpose a Matrix4.
 *
 * @param m - the matrix that will have it's columns and rows flipped.
 * @return - a matrix with columns and rows flipped from the input.
 */
struct Matrix4 Matrix4_transpose(struct Matrix4 const *m);

/**
 * Return a Matrix4 that is all zeros.
 *
 * @return - a Matrix4 with all elements initialized to zero.
 */
struct Matrix4 Matrix4_zero();

#endif
