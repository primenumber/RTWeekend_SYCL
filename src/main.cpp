#include <fstream>
#include <iostream>
#include <string>
#include <sycl/sycl.hpp>
#include <vector>

#include "color.hpp"
#include "ray.hpp"
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

color ray_color(const ray &r) {
  vec3 unit_direction = normalize(r.dir);
  auto t = 0.5f * (unit_direction.y() + 1.0f);
  return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

int main() {
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const size_t image_width = 400;
  const size_t image_height = static_cast<size_t>(image_width / aspect_ratio);
  Image image(image_height, image_width);

  // Camera
  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0;

  auto origin = point3(0, 0, 0);
  auto horizontal = vec3(viewport_width, 0, 0);
  auto vertical = vec3(0, viewport_height, 0);
  auto lower_left_corner =
      origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

  // Buffer
  sycl::queue q;
  auto image_buf = sycl::malloc_shared<color>(image_height * image_width, q);

  // Render
  q.submit([&](sycl::handler &h) {
     h.parallel_for(sycl::range<2>(image_height, image_width), [=](auto &idx) {
       size_t i = image_height - idx[0] - 1;
       size_t j = idx[1];
       auto u = float(j) / (image_width - 1);
       auto v = float(i) / (image_height - 1);
       ray r(origin,
             lower_left_corner + u * horizontal + v * vertical - origin);
       color pixel_color = ray_color(r);

       size_t offset = idx[0] * image_width + idx[1];
       image_buf[offset] = pixel_color;
     });
   }).wait();
  for (size_t i = 0; i < image_height * image_width; ++i) {
    image.data[i] = image_buf[i];
  }

  // Show
  std::cout << image;
  return 0;
}
