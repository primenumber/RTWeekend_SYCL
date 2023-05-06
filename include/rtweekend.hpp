#pragma once

#include <cmath>
#include <limits>
#include <memory>

constexpr float infinity = std::numeric_limits<float>::infinity();
const float pi = std::acos(-1);

inline float degrees_to_radians(float degrees) { return degrees * pi / 180.0f; }

#include "ray.hpp"
#include "vec3.hpp"
