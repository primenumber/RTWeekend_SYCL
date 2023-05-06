#pragma once
#include <variant>

#include "ray.hpp"
#include "vec3.hpp"

struct hit_record {
  point3 p;
  vec3 normal;
  float t;
  bool front_face;

  void set_face_normal(const ray &r, const vec3 &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

#include "sphere.hpp"

using hittable = std::variant<sphere>;

inline bool hit(const hittable &h, const ray &r, float t_min, float t_max,
                hit_record &rec) {
  if (auto ptr = std::get_if<sphere>(&h); ptr) {
    return ptr->hit(r, t_min, t_max, rec);
  } else {
    return false;
  }
}
