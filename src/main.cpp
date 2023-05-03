#include <fstream>
#include <iostream>
#include <string>
#include <sycl/sycl.hpp>
#include <vector>

// Image

struct Image {
  Image(size_t height, size_t width, size_t channel = 3)
      : data(height * width * channel), height(height), width(width),
        channel(channel) {}
  std::vector<int> data;
  const size_t height;
  const size_t width;
  const size_t channel;
};

std::ostream &operator<<(std::ostream &os, const Image &image) {
  os << "P3\n" << image.width << " " << image.height << "\n255\n";
  for (size_t i = 0; i < image.height * image.width; ++i) {
    int r = image.data[i * 3 + 0];
    int g = image.data[i * 3 + 1];
    int b = image.data[i * 3 + 2];
    os << r << ' ' << g << ' ' << b << '\n';
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

      int ir = static_cast<int>(255.999 * r);
      int ig = static_cast<int>(255.999 * g);
      int ib = static_cast<int>(255.999 * b);
      image.data[offset + 0] = ir;
      image.data[offset + 1] = ig;
      image.data[offset + 2] = ib;
      offset += 3;
    }
  }
  std::cerr << "\nDone.\n";
  std::cout << image;
  return 0;
}
