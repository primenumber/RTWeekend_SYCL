#pragma once
#include <iostream>

#include "vec3.hpp"

void write_color(std::ostream &out, const color &pixel_color) {
  out << static_cast<int>(255.999 * pixel_color.x()) << ' '
      << static_cast<int>(255.999 * pixel_color.y()) << ' '
      << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}