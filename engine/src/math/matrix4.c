#include "math/matrix4.h"

#include <math.h>

float Matrix4_determinant(struct Matrix4 const *m) {
  float d1 = m->f22 * (m->f33 * m->f44 - m->f34 * m->f43) -
             m->f23 * (m->f32 * m->f44 - m->f42 * m->f34) +
             m->f24 * (m->f32 * m->f43 - m->f42 * m->f33);
  float d2 = m->f21 * (m->f33 * m->f44 - m->f43 * m->f34) -
             m->f23 * (m->f31 * m->f44 - m->f34 * m->f41) +
             m->f24 * (m->f31 * m->f43 - m->f33 * m->f41);
  float d3 = m->f21 * (m->f32 * m->f44 - m->f42 * m->f34) -
             m->f22 * (m->f31 * m->f44 - m->f34 * m->f41) +
             m->f24 * (m->f31 * m->f42 - m->f32 * m->f41);
  float d4 = m->f21 * (m->f32 * m->f43 - m->f42 * m->f34) -
             m->f22 * (m->f31 * m->f43 - m->f33 * m->f41) +
             m->f23 * (m->f31 * m->f42 - m->f32 * m->f41);

  return m->f11 * d1 - m->f12 * d2 + m->f13 * d3 - m->f14 * d4;
}

struct Matrix4 Matrix4_frustum(float left, float right, float bottom, float top,
                               float near, float far) {
  struct Matrix4 r = Matrix4_identity();

  r.f11 = (2.f * near) / (right - left);
  r.f13 = (right + left) / (right - left);
  r.f22 = (2.f * near) / (top - bottom);
  r.f23 = (top + bottom) / (top - bottom);
  r.f33 = -(far + near) / (far - near);
  r.f34 = (-2.f * far * near) / (far - near);
  r.f43 = -1.f;

  return r;
}

struct Vector4 Matrix4_get_column1(struct Matrix4 const *m) {
  return (struct Vector4){m->f11, m->f12, m->f13, m->f14};
}

struct Vector4 Matrix4_get_column2(struct Matrix4 const *m) {
  return (struct Vector4){m->f21, m->f22, m->f23, m->f24};
}

struct Vector4 Matrix4_get_column3(struct Matrix4 const *m) {
  return (struct Vector4){m->f31, m->f32, m->f33, m->f34};
}

struct Vector4 Matrix4_get_column4(struct Matrix4 const *m) {
  return (struct Vector4){m->f41, m->f42, m->f43, m->f44};
}

struct Vector4 Matrix4_get_row1(struct Matrix4 const *m) {
  return (struct Vector4){m->f11, m->f21, m->f31, m->f41};
}

struct Vector4 Matrix4_get_row2(struct Matrix4 const *m) {
  return (struct Vector4){m->f12, m->f22, m->f32, m->f42};
}

struct Vector4 Matrix4_get_row3(struct Matrix4 const *m) {
  return (struct Vector4){m->f13, m->f23, m->f33, m->f43};
}

struct Vector4 Matrix4_get_row4(struct Matrix4 const *m) {
  return (struct Vector4){m->f14, m->f24, m->f34, m->f44};
}

struct Matrix4 Matrix4_identity() {
  struct Matrix4 m = Matrix4_zero();
  m.f11 = 1.f;
  m.f22 = 1.f;
  m.f33 = 1.f;
  m.f44 = 1.f;
  return m;
}

// This is taken from:
// http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
struct Matrix4 Matrix4_invert(struct Matrix4 const *m) {
  float det = Matrix4_determinant(m);
  if (det == 0.f) {
    return Matrix4_identity();
  }

  // This macro is a simple helper to define the intermediate matrix inverse
  // components.
#define b(m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15,    \
          m16, m17, m18)                                                       \
  m->f##m1 * m->f##m2 * m->f##m3 + m->f##m4 * m->f##m5 * m->f##m6 +            \
      m->f##m7 * m->f##m8 * m->f##m9 - m->f##m10 * m->f##m11 * m->f##m12 -     \
      m->f##m13 * m->f##m14 * m->f##m15 - m->f##m16 * m->f##m17 * m->f##m18

  float b11 =
      b(22, 33, 44, 23, 34, 42, 24, 32, 43, 22, 34, 43, 23, 32, 44, 24, 33, 42);
  float b12 =
      b(12, 34, 43, 13, 32, 44, 14, 33, 42, 12, 33, 44, 13, 34, 42, 14, 32, 43);
  float b13 =
      b(12, 23, 44, 13, 24, 42, 14, 22, 43, 12, 24, 43, 13, 22, 44, 14, 23, 42);
  float b14 =
      b(12, 24, 33, 13, 22, 34, 14, 23, 32, 12, 23, 34, 13, 24, 32, 14, 22, 33);
  float b21 =
      b(21, 34, 43, 23, 31, 44, 24, 33, 41, 21, 33, 44, 23, 34, 41, 24, 31, 43);
  float b22 =
      b(11, 33, 44, 13, 34, 41, 14, 31, 43, 11, 34, 43, 13, 31, 44, 14, 33, 41);
  float b23 =
      b(11, 24, 43, 13, 21, 44, 14, 23, 41, 11, 23, 44, 13, 24, 41, 14, 21, 43);
  float b24 =
      b(11, 23, 34, 13, 24, 31, 14, 21, 33, 11, 24, 33, 13, 21, 34, 14, 23, 31);
  float b31 =
      b(21, 32, 44, 22, 34, 41, 24, 31, 42, 21, 34, 42, 22, 31, 44, 24, 32, 41);
  float b32 =
      b(11, 34, 42, 12, 31, 44, 14, 32, 41, 11, 32, 44, 12, 34, 41, 14, 31, 42);
  float b33 =
      b(11, 22, 44, 12, 24, 41, 14, 21, 42, 11, 24, 42, 12, 21, 44, 14, 22, 41);
  float b34 =
      b(11, 24, 32, 12, 21, 34, 14, 22, 31, 11, 22, 34, 12, 24, 31, 14, 21, 32);
  float b41 =
      b(21, 33, 42, 22, 31, 43, 23, 32, 41, 21, 32, 43, 22, 33, 41, 23, 31, 42);
  float b42 =
      b(11, 32, 43, 12, 33, 41, 13, 31, 42, 11, 33, 42, 12, 31, 43, 13, 32, 41);
  float b43 =
      b(11, 23, 42, 12, 21, 43, 13, 22, 41, 11, 22, 43, 12, 23, 41, 13, 21, 42);
  float b44 =
      b(11, 22, 33, 12, 23, 31, 13, 21, 32, 11, 23, 32, 12, 21, 33, 13, 22, 31);

#undef b

  float inv_det = 1.f / det;

  struct Matrix4 b;
  b.f11 = b11 * inv_det;
  b.f12 = b12 * inv_det;
  b.f13 = b13 * inv_det;
  b.f14 = b14 * inv_det;
  b.f21 = b21 * inv_det;
  b.f22 = b22 * inv_det;
  b.f23 = b23 * inv_det;
  b.f24 = b24 * inv_det;
  b.f31 = b31 * inv_det;
  b.f32 = b32 * inv_det;
  b.f33 = b33 * inv_det;
  b.f34 = b34 * inv_det;
  b.f41 = b41 * inv_det;
  b.f42 = b42 * inv_det;
  b.f43 = b43 * inv_det;
  b.f44 = b44 * inv_det;

  return b;
}

struct Matrix4 Matrix4_lookat(struct Vector4 eye, struct Vector4 target,
                              struct Vector4 up) {
  struct Vector4 f = Vector4_subtract(target, eye);
  Vector4_normalize(&f);

  struct Vector4 s = Vector4_cross(f, up);
  Vector4_normalize(&s);

  struct Vector4 u = Vector4_cross(s, f);

  struct Matrix4 r = Matrix4_identity();
  r.f11 = s.x;
  r.f21 = s.y;
  r.f31 = s.z;
  r.f12 = u.x;
  r.f22 = u.y;
  r.f32 = u.z;
  r.f13 = -f.x;
  r.f23 = -f.y;
  r.f33 = -f.z;
  r.f41 = -Vector4_dot(s, eye);
  r.f42 = -Vector4_dot(u, eye);
  r.f43 = Vector4_dot(f, eye);

  return r;
}

struct Matrix4 Matrix4_multiply(struct Matrix4 const *a,
                                struct Matrix4 const *b) {
  struct Matrix4 m;

  /* I think this is wrong becuase I goofed the memory ordering
  // row 1
  m.f11 = a->f11 * b->f11 + a->f12 * b->f21 + a->f13 * b->f31 + a->f14 * b->f41;
  m.f12 = a->f11 * b->f12 + a->f12 * b->f22 + a->f13 * b->f32 + a->f14 * b->f42;
  m.f13 = a->f11 * b->f13 + a->f12 * b->f23 + a->f13 * b->f33 + a->f14 * b->f43;
  m.f14 = a->f11 * b->f14 + a->f12 * b->f24 + a->f13 * b->f34 + a->f14 * b->f44;

  // row 2
  m.f21 = a->f21 * b->f11 + a->f22 * b->f21 + a->f23 * b->f31 + a->f24 * b->f41;
  m.f22 = a->f21 * b->f12 + a->f22 * b->f22 + a->f23 * b->f32 + a->f24 * b->f42;
  m.f23 = a->f21 * b->f13 + a->f22 * b->f23 + a->f23 * b->f33 + a->f24 * b->f43;
  m.f24 = a->f21 * b->f14 + a->f22 * b->f24 + a->f23 * b->f34 + a->f24 * b->f44;

  // row 3
  m.f31 = a->f31 * b->f11 + a->f32 * b->f21 + a->f33 * b->f31 + a->f34 * b->f41;
  m.f32 = a->f31 * b->f12 + a->f32 * b->f22 + a->f33 * b->f32 + a->f34 * b->f42;
  m.f33 = a->f31 * b->f13 + a->f32 * b->f23 + a->f33 * b->f33 + a->f34 * b->f43;
  m.f34 = a->f31 * b->f14 + a->f32 * b->f24 + a->f33 * b->f34 + a->f34 * b->f44;

  // row 4
  m.f41 = a->f41 * b->f11 + a->f42 * b->f21 + a->f43 * b->f31 + a->f44 * b->f41;
  m.f42 = a->f41 * b->f12 + a->f42 * b->f22 + a->f43 * b->f32 + a->f44 * b->f42;
  m.f43 = a->f41 * b->f13 + a->f42 * b->f23 + a->f43 * b->f33 + a->f44 * b->f43;
  m.f44 = a->f41 * b->f14 + a->f42 * b->f24 + a->f43 * b->f34 + a->f44 * b->f44;
  */
  m.f11 = a->f11 * b->f11 + a->f21 * b->f12 + a->f31 * b->f13 + a->f41 * b->f14;
  m.f21 = a->f11 * b->f21 + a->f21 * b->f22 + a->f31 * b->f23 + a->f41 * b->f24;
  m.f31 = a->f11 * b->f31 + a->f21 * b->f32 + a->f31 * b->f33 + a->f41 * b->f34;
  m.f41 = a->f11 * b->f41 + a->f21 * b->f42 + a->f31 * b->f43 + a->f41 * b->f44;

  m.f12 = a->f12 * b->f11 + a->f22 * b->f12 + a->f32 * b->f13 + a->f42 * b->f14;
  m.f22 = a->f12 * b->f21 + a->f22 * b->f22 + a->f32 * b->f23 + a->f42 * b->f24;
  m.f32 = a->f12 * b->f31 + a->f22 * b->f32 + a->f32 * b->f33 + a->f42 * b->f34;
  m.f42 = a->f12 * b->f41 + a->f22 * b->f42 + a->f32 * b->f43 + a->f42 * b->f44;

  m.f13 = a->f13 * b->f11 + a->f23 * b->f12 + a->f33 * b->f13 + a->f43 * b->f14;
  m.f23 = a->f13 * b->f21 + a->f23 * b->f22 + a->f33 * b->f23 + a->f43 * b->f24;
  m.f33 = a->f13 * b->f31 + a->f23 * b->f32 + a->f33 * b->f33 + a->f43 * b->f34;
  m.f43 = a->f13 * b->f41 + a->f23 * b->f42 + a->f33 * b->f43 + a->f43 * b->f44;

  m.f14 = a->f14 * b->f11 + a->f24 * b->f12 + a->f34 * b->f13 + a->f44 * b->f14;
  m.f24 = a->f14 * b->f21 + a->f24 * b->f22 + a->f34 * b->f23 + a->f44 * b->f24;
  m.f34 = a->f14 * b->f31 + a->f24 * b->f32 + a->f34 * b->f33 + a->f44 * b->f34;
  m.f44 = a->f14 * b->f41 + a->f24 * b->f42 + a->f34 * b->f43 + a->f44 * b->f44;
  return m;
}

struct Matrix4 Matrix4_orthographic(float left, float right, float bottom,
                                    float top) {
  return Matrix4_orthographic_nf(left, right, bottom, top, -1.f, 1.f);
}

struct Matrix4 Matrix4_orthographic_nf(float left, float right, float bottom,
                                       float top, float near, float far) {
  struct Matrix4 r = Matrix4_identity();

  r.f11 = 2.f / (right - left);
  r.f14 = -((right + left) / (right - left));
  r.f22 = 2.f / (top - bottom);
  r.f24 = -((top + bottom) / (top - bottom));
  r.f33 = -2.f / (far - near);
  r.f34 = -((far + near) / (far - near));

  return r;
}

struct Matrix4 Matrix4_perspective(float fovy, float aspect_ratio, float near,
                                   float far) {
  struct Matrix4 r = Matrix4_zero();

  float tanFovy2 = tanf(fovy / 2.f);

  r.f11 = 1.f / (aspect_ratio * tanFovy2);
  r.f22 = 1.f / tanFovy2;
  r.f33 = -(far + near) / (far - near);
  r.f34 = -1.f;
  r.f43 = -(2.f * far * near) / (far - near);

  return r;
}

struct Matrix4 Matrix4_rotation_x(float angle) {
  struct Matrix4 r = Matrix4_identity();

  r.f22 = cosf(angle);
  r.f32 = sinf(angle);
  r.f23 = -sinf(angle);
  r.f33 = cosf(angle);

  return r;
}

struct Matrix4 Matrix4_rotation_y(float angle) {
  struct Matrix4 r = Matrix4_identity();

  r.f11 = cosf(angle);
  r.f31 = -sinf(angle);
  r.f13 = sinf(angle);
  r.f33 = cosf(angle);

  return r;
}

struct Matrix4 Matrix4_rotation_z(float angle) {
  struct Matrix4 r = Matrix4_identity();

  r.f11 = cosf(angle);
  r.f21 = sinf(angle);
  r.f12 = -sinf(angle);
  r.f22 = cosf(angle);

  return r;
}

void Matrix4_set_column1(struct Matrix4 *m, struct Vector4 v) {
  m->f11 = v.x;
  m->f12 = v.y;
  m->f13 = v.z;
  m->f14 = v.w;
}

void Matrix4_set_column2(struct Matrix4 *m, struct Vector4 v) {
  m->f21 = v.x;
  m->f22 = v.y;
  m->f23 = v.z;
  m->f24 = v.w;
}

void Matrix4_set_column3(struct Matrix4 *m, struct Vector4 v) {
  m->f31 = v.x;
  m->f32 = v.y;
  m->f33 = v.z;
  m->f34 = v.w;
}

void Matrix4_set_column4(struct Matrix4 *m, struct Vector4 v) {
  m->f41 = v.x;
  m->f42 = v.y;
  m->f43 = v.z;
  m->f44 = v.w;
}

void Matrix4_set_row1(struct Matrix4 *m, struct Vector4 v) {
  m->f11 = v.x;
  m->f21 = v.y;
  m->f31 = v.z;
  m->f41 = v.w;
}

void Matrix4_set_row2(struct Matrix4 *m, struct Vector4 v) {
  m->f12 = v.x;
  m->f22 = v.y;
  m->f32 = v.z;
  m->f42 = v.w;
}

void Matrix4_set_row3(struct Matrix4 *m, struct Vector4 v) {
  m->f13 = v.x;
  m->f23 = v.y;
  m->f33 = v.z;
  m->f43 = v.w;
}

void Matrix4_set_row4(struct Matrix4 *m, struct Vector4 v) {
  m->f14 = v.x;
  m->f24 = v.y;
  m->f34 = v.z;
  m->f44 = v.w;
}

struct Matrix4 Matrix4_scale(float x, float y, float z) {
  struct Matrix4 m = Matrix4_identity();

  m.f11 = x;
  m.f22 = y;
  m.f33 = z;

  return m;
}

struct Vector4 Matrix4_transform(struct Matrix4 const *m, struct Vector4 v) {
  struct Vector4 r;

  r.x = m->f11 * v.x + m->f21 * v.y + m->f31 * v.z + m->f41 * v.w;
  r.y = m->f12 * v.x + m->f22 * v.y + m->f32 * v.z + m->f42 * v.w;
  r.z = m->f13 * v.x + m->f23 * v.y + m->f33 * v.z + m->f43 * v.w;
  r.w = m->f14 * v.x + m->f24 * v.y + m->f34 * v.z + m->f44 * v.w;

  return r;
}

struct Matrix4 Matrix4_translation(float x, float y, float z) {
  struct Matrix4 m = Matrix4_identity();

  m.f41 = x;
  m.f42 = y;
  m.f43 = z;

  return m;
}

// TODO: This is a wasteful implementation.
// There are a bumch of copies that can be removed.
struct Matrix4 Matrix4_transpose(struct Matrix4 const *m) {
  struct Matrix4 r;

  Matrix4_set_row1(&r, Matrix4_get_column1(m));
  Matrix4_set_row2(&r, Matrix4_get_column2(m));
  Matrix4_set_row3(&r, Matrix4_get_column3(m));
  Matrix4_set_row4(&r, Matrix4_get_column4(m));

  return r;
}

struct Matrix4 Matrix4_zero() {
  struct Matrix4 m;
  m.f11 = 0.f;
  m.f21 = 0.f;
  m.f31 = 0.f;
  m.f41 = 0.f;
  m.f12 = 0.f;
  m.f22 = 0.f;
  m.f32 = 0.f;
  m.f42 = 0.f;
  m.f13 = 0.f;
  m.f23 = 0.f;
  m.f33 = 0.f;
  m.f43 = 0.f;
  m.f14 = 0.f;
  m.f24 = 0.f;
  m.f34 = 0.f;
  m.f44 = 0.f;
  return m;
}
