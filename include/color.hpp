#pragma once
#include <iostream>

#include "vec3.hpp"

void write_color(std::ostream &out, const color &pixel_color,
                 size_t samples_per_pixel) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  auto scale = 1.0f / samples_per_pixel;
  r = pow(r * scale, 1 / 2.2);
  g = pow(g * scale, 1 / 2.2);
  b = pow(b * scale, 1 / 2.2);

  out << static_cast<int>(255.999 * std::clamp(r, 0.0f, 0.999f)) << ' '
      << static_cast<int>(255.999 * std::clamp(g, 0.0f, 0.999f)) << ' '
      << static_cast<int>(255.999 * std::clamp(b, 0.0f, 0.999f)) << '\n';
}
