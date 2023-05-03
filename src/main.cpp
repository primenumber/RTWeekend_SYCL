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
  auto image_buf = sycl::malloc_shared<color>(image_height * image_width, q);
  q.submit([&](sycl::handler &h) {
     h.parallel_for(sycl::range<2>(image_height, image_width), [=](auto &idx) {
       size_t i = image_height - idx[0] - 1;
       size_t j = idx[1];
       auto r = float(j) / (image_width - 1);
       auto g = float(i) / (image_height - 1);
       auto b = 0.25;

       size_t offset = idx[0] * image_width + idx[1];
       image_buf[offset] = color(r, g, b);
     });
   }).wait();
  for (size_t i = 0; i < image_height * image_width; ++i) {
    image.data[i] = image_buf[i];
  }
  std::cout << image;
  return 0;
}
