#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

constexpr float infinity = std::numeric_limits<float>::infinity();
const float pi = std::acos(-1);

inline float degrees_to_radians(float degrees) { return degrees * pi / 180.0f; }

struct xorshift32 {
  xorshift32(uint32_t s = 0xdeadbeef) : s(s) {}
  uint32_t next() {
    uint32_t x = s;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return s = x;
  }
  uint32_t s;
};

inline float random_float(xorshift32 &gen) {
  uint32_t v = gen.next();
  return v / 4294967296.f;
}

inline float random_float(xorshift32 &gen, float min, float max) {
  return min + (max - min) * random_float(gen);
}

#include "ray.hpp"
#include "vec3.hpp"
