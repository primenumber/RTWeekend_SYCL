#include <fstream>
#include <iostream>
#include <string>
#include <sycl/sycl.hpp>
#include <vector>

#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "rtweekend.hpp"
#include "sphere.hpp"

// Image

struct Image {
  Image(size_t height, size_t width)
      : data(height * width), height(height), width(width) {}
  std::vector<color> data;
  const size_t height;
  const size_t width;
};

void write_image(std::ostream &os, const Image &image,
                 size_t samples_per_pixel) {
  os << "P3\n" << image.width << " " << image.height << "\n255\n";
  for (size_t i = 0; i < image.height * image.width; ++i) {
    write_color(os, image.data.at(i), samples_per_pixel);
  }
}

color ray_color(const ray &r, const hittable_list &world) {
  hit_record rec;
  if (world.hit(r, 0, infinity, rec)) {
    return 0.5f * (rec.normal + color(1, 1, 1));
  }
  vec3 unit_direction = normalize(r.dir);
  auto t = 0.5f * (unit_direction.y() + 1.0f);
  return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

void write_vec3(std::ostream &os, const vec3 &v) {
  os << v.x() << ',' << v.y() << ',' << v.z() << '\n';
}

int main() {
  sycl::queue q;
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const size_t image_width = 400;
  const size_t image_height = static_cast<size_t>(image_width / aspect_ratio);
  const size_t samples_per_pixel = 100;
  Image image(image_height, image_width);

  // World
  hittable_list world;
  world.ptr = sycl::malloc_shared<hittable>(2, q);
  world.ptr[0] = sphere(point3(0, 0, -1), 0.5f);
  world.ptr[1] = sphere(point3(0, -100.5f, -1), 100);
  world.length = 2;

  // Camera
  camera cam;

  // Buffer
  auto image_buf = sycl::malloc_shared<color>(image_height * image_width, q);

  // Render
  q.submit([&](sycl::handler &h) {
     h.parallel_for(sycl::range<2>(image_height, image_width), [=](auto &idx) {
       xorshift32 gen;
       size_t i = image_height - idx[0] - 1;
       size_t j = idx[1];
       color pixel_color(0, 0, 0);
       for (size_t s = 0; s < samples_per_pixel; ++s) {
         auto u = (j + random_float(gen)) / (image_width - 1);
         auto v = (i + random_float(gen)) / (image_height - 1);
         ray r = cam.get_ray(u, v);
         pixel_color += ray_color(r, world);
       }

       size_t offset = idx[0] * image_width + idx[1];
       image_buf[offset] = pixel_color;
     });
   }).wait();
  for (size_t i = 0; i < image_height * image_width; ++i) {
    image.data[i] = image_buf[i];
  }

  // Show
  write_image(std::cout, image, samples_per_pixel);
  sycl::free(image_buf, q);
  sycl::free(world.ptr, q);
  return 0;
}
