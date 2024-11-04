#include "math/vector4.h"

#include <math.h>

struct Vector4 Vector4_new_point(float x, float y, float z) {
  return (struct Vector4){x, y, z, 1.f};
}

struct Vector4 Vector4_new_vector(float x, float y, float z) {
  return (struct Vector4){x, y, z, 0.f};
}

struct Vector4 Vector4_zero_point() {
  return (struct Vector4){0.f, 0.f, 0.f, 1.f};
}

struct Vector4 Vector4_zero_vector() {
  return (struct Vector4){0.f, 0.f, 0.f, 0.f};
}

struct Vector4 Vector4_one_point() {
  return (struct Vector4){1.f, 1.f, 1.f, 1.f};
}

struct Vector4 Vector4_one_vector() {
  return (struct Vector4){1.f, 1.f, 1.f, 0.f};
}

float Vector4_magnitude(struct Vector4 v) {
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

float Vector4_magnitude_squared(struct Vector4 v) {
  return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

struct Vector4 Vector4_normalized(struct Vector4 v) {
  float magnitude = Vector4_magnitude(v);
  if (magnitude == 0.f) {
    return (struct Vector4){0.f, 0.f, 0.f, 0.f};
  }
  return (struct Vector4){v.x / magnitude, v.y / magnitude, v.z / magnitude,
                          v.w / magnitude};
}

void Vector4_normalize(struct Vector4 *v) {
  float magnitude = Vector4_magnitude(*v);
  if (magnitude == 0.f) {
    v->x = 0.f;
    v->y = 0.f;
    v->z = 0.f;
    v->w = 0.f;
    return;
  }
  v->x /= magnitude;
  v->y /= magnitude;
  v->z /= magnitude;
  v->w /= magnitude;
}

float Vector4_distance(struct Vector4 a, struct Vector4 b) {
  struct Vector4 dist = Vector4_subtract(a, b);
  return Vector4_magnitude(dist);
}

float Vector4_distance_squared(struct Vector4 a, struct Vector4 b) {
  struct Vector4 dist = Vector4_subtract(a, b);
  return Vector4_magnitude_squared(dist);
}

float Vector4_dot(struct Vector4 a, struct Vector4 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

struct Vector4 Vector4_cross(struct Vector4 a, struct Vector4 b) {
  return (struct Vector4){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                          a.x * b.y - a.y * b.x, 0.0f};
}

struct Vector4 Vector4_lerp(struct Vector4 a, struct Vector4 b, float t) {
  // a + (b - a) * t
  // potentially switch this for precise lerp later.
  struct Vector4 difference = Vector4_subtract(b, a);
  struct Vector4 scaled = Vector4_scale(difference, t);
  return Vector4_add(a, scaled);
}

struct Vector4 Vector4_add(struct Vector4 a, struct Vector4 b) {
  return (struct Vector4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

struct Vector4 Vector4_subtract(struct Vector4 a, struct Vector4 b) {
  return (struct Vector4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

struct Vector4 Vector4_scale(struct Vector4 v, float s) {
  return (struct Vector4){v.x * s, v.y * s, v.z * s, v.w * s};
}
