#include "math/quaternion.h"
#include "math/utilities.h"

#include <math.h>

struct Quaternion Quaternion_new(float x, float y, float z, float w) {
  return (struct Quaternion){x, y, z, w};
}

struct Quaternion Quaternion_new_euler(float x, float y, float z) {
  float cos_x = cosf(x / 2.f);
  float cos_y = cosf(y / 2.f);
  float cos_z = cosf(z / 2.f);
  float sin_x = sinf(x / 2.f);
  float sin_y = sinf(y / 2.f);
  float sin_z = sinf(z / 2.f);

  return Quaternion_new(cos_x * cos_y * sin_z - sin_x * sin_y * cos_z,
                        cos_x * sin_y * cos_z + sin_x * cos_y * sin_z,
                        sin_x * cos_y * cos_z - cos_x * sin_y * sin_z,
                        cos_x * cos_y * cos_z + sin_x * sin_y * sin_z);
}

struct Quaternion Quaternion_new_axis_angle(struct Vector4 axis, float angle) {
  float sin_angle = sinf(angle / 2.f);
  float cos_angle = cosf(angle / 2.f);

  return Quaternion_normalized(Quaternion_new(
      axis.x * sin_angle, axis.y * sin_angle, axis.z * sin_angle, cos_angle));
}

struct Quaternion Quaternion_identity() {
  return Quaternion_new(0.f, 0.f, 0.f, 1.f);
}

void Quaternion_to_axis_angle(struct Quaternion q, struct Vector4 *axis,
                              float *angle) {
  float a = acosf(q.w);
  *angle = 2.f * a;

  float divisor = sinf(a);
  if (EPSILON_COMPARE(divisor, 0.f)) {
    axis->x = 0.f;
    axis->y = 1.f;
    axis->z = 0.f;
    axis->w = 0.f;
  } else {
    *axis = Vector4_new_vector(q.x / divisor, q.y / divisor, q.z / divisor);
    Vector4_normalize(axis);
  }
}

struct Vector4 Quaternion_to_euler(struct Quaternion q) {
  float sx = q.x * q.x;
  float sy = q.y * q.y;
  float sz = q.z * q.z;
  float sw = q.w * q.w;

  return Vector4_new_vector(
      asinf(-2.f * (q.x * q.z - q.y * q.w)),
      atan2f(2.f * (q.y * q.z + q.x * q.w), -sx - sy + sz + sw),
      atan2f(2.f * (q.x * q.y + q.z * q.w), sx - sy - sz + sw));
}

struct Quaternion Quaternion_multiply(struct Quaternion a,
                                      struct Quaternion b) {
  return Quaternion_new((a.w * b.x) + (a.x * b.w) + (a.y * b.z) - (a.z * b.y),
                        (a.w * b.y) - (a.x * b.z) + (a.y * b.w) + (a.z * b.x),
                        (a.w * b.z) + (a.x * b.y) - (a.y * b.x) + (a.z * b.w),
                        (a.w * b.w) - (a.x * b.x) - (a.y * b.y) - (a.z * b.z));
}

struct Vector4 Quaternion_transform(struct Quaternion q, struct Vector4 v) {
  struct Quaternion temp = q;
  temp = Quaternion_multiply(
      temp, Quaternion_normalized(Quaternion_new(v.x, v.y, v.z, 0.0)));
  temp = Quaternion_multiply(temp, Quaternion_inverse(q));

  struct Vector4 result = Vector4_new_vector(temp.x, temp.y, temp.z);

  return Vector4_scale(result, Vector4_magnitude(v));
}

struct Quaternion Quaternion_inverse(struct Quaternion q) {
  float scale = Quaternion_length(q);
  struct Quaternion result = Quaternion_inverse_unit(q);

  if (EPSILON_COMPARE(scale, 0.f)) {
    result.x /= scale;
    result.y /= scale;
    result.z /= scale;
    result.w /= scale;
  }

  return result;
}

struct Quaternion Quaternion_inverse_unit(struct Quaternion q) {
  return Quaternion_new(-q.x, -q.y, -q.z, q.w);
}

float Quaternion_length(struct Quaternion q) {
  return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

struct Quaternion Quaternion_normalized(struct Quaternion q) {
  float scale = Quaternion_length(q);

  if (EPSILON_COMPARE(scale, 0.f)) {
    return Quaternion_new(q.x / scale, q.y / scale, q.z / scale, q.w / scale);
  }

  return Quaternion_new(0.f, 0.f, 0.f, 0.f);
}

struct Quaternion Quaternion_slerp(struct Quaternion a, struct Quaternion b,
                                   float t) {
  float scale0, scale1;
  struct Vector4 afb1;
  float cosom = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

  if (cosom < 0.0f) {
    cosom = -cosom;
    afb1.x = -b.x;
    afb1.y = -b.y;
    afb1.z = -b.z;
    afb1.w = -b.w;
  } else {
    afb1.x = b.x;
    afb1.y = b.y;
    afb1.z = b.z;
    afb1.w = b.w;
  }

  const float Quaternion_DELTA_COS_MIN = 0.01f;

  if ((1.0f - cosom) > Quaternion_DELTA_COS_MIN) {
    float omega = acosf(cosom);
    float sinom = sinf(omega);
    scale0 = sinf((1.0f - t) * omega) / sinom;
    scale1 = sinf(t * omega) / sinom;
  } else {
    // regress to a linear interpolation if the Quaternions are very close
    // together.
    scale0 = 1.0f - t;
    scale1 = t;
  }

  return Quaternion_new(
      (scale0 * a.x) + (scale1 * afb1.x), (scale0 * a.y) + (scale1 * afb1.y),
      (scale0 * a.z) + (scale1 * afb1.z), (scale0 * a.w) + (scale1 * afb1.w));
}

float Quaternion_dot(struct Quaternion a, struct Quaternion b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
