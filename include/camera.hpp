#pragma once
#include "rtweekend.hpp"

class camera {
public:
  camera() {
    const auto aspect_ratio = 16.0f / 9.0f;
    auto viewport_height = 2.0f;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0f;

    origin = point3(0, 0, 0);
    horizontal = vec3(viewport_width, 0, 0);
    vertical = vec3(0, viewport_height, 0);
    lower_left_corner =
        origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
  }

  ray get_ray(float u, float v) const {
    return ray(origin,
               lower_left_corner + u * horizontal + v * vertical - origin);
  }

public:
  point3 origin;
  point3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
};
