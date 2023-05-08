#pragma once
#include <cmath>
#include <iostream>

class vec3 {
public:
  vec3() : e{0, 0, 0} {}
  vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

  float x() const { return e[0]; }
  float y() const { return e[1]; }
  float z() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  float operator[](size_t i) const { return e[i]; }
  float &operator[](size_t i) { return e[i]; }

  vec3 &operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  vec3 &operator*=(const float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3 &operator/=(const float t) { return *this *= 1 / t; }

  float length() const { return sycl::sqrt(length_squared()); }

  float length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  static vec3 random(xorshift32 &gen) {
    return vec3(random_float(gen), random_float(gen), random_float(gen));
  }

  static vec3 random(xorshift32 &gen, float min, float max) {
    return vec3(random_float(gen, min, max), random_float(gen, min, max),
                random_float(gen, min, max));
  }

public:
  float e[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const float t, const vec3 &v) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, float t) { return t * v; }

inline vec3 operator/(const vec3 &v, float t) { return (1 / t) * v; }

inline float dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 normalize(const vec3 &v) { return v / v.length(); }

inline vec3 random_in_unit_sphere(xorshift32 &gen) {
  auto y = random_float(gen, -1, 1);
  auto r = sycl::sqrt(1.0f - y * y);
  auto theta = random_float(gen, 0, 2.0f * sycl::acos(-1.0f));
  return vec3(r * sycl::cos(theta), y, r * sycl::sin(theta));
}

inline vec3 random_unit_vector(xorshift32 &gen) {
  return normalize(random_in_unit_sphere(gen));
}
