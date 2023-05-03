#include <fstream>
#include <iostream>
#include <string>
#include <sycl/sycl.hpp>
#include <vector>

#include "color.hpp"
#include "vec3.hpp"

// Image

struct Image {
  Image(size_t height, size_t width)
      : data(height * width), height(height), width(width) {}
  std::vector<color> data;
  const size_t height;
  const size_t width;
};

std::ostream &operator<<(std::ostream &os, const Image &image) {
  os << "P3\n" << image.width << " " << image.height << "\n255\n";
  for (size_t i = 0; i < image.height * image.width; ++i) {
    write_color(os, image.data.at(i));
  }
  return os;
}

int main() {
  sycl::queue q;
  const size_t image_height = 256;
  const size_t image_width = 256;
  Image image(image_height, image_width);
  size_t offset = 0;
  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      auto r = double(i) / (image_width - 1);
      auto g = double(j) / (image_height - 1);
      auto b = 0.25;

      image.data[offset] = color(r, g, b);
      ++offset;
    }
  }
  std::cerr << "\nDone.\n";
  std::cout << image;
  return 0;
}
