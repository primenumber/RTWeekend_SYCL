#pragma once
#include "hittable.hpp"

struct hittable_list {
public:
  hittable_list() : ptr(nullptr), length(0) {}
  size_t size() const { return length; }
  const hittable &operator[](const size_t index) const { return ptr[index]; }
  hittable &operator[](const size_t index) { return ptr[index]; }
  bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    hit_record tmp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (size_t i = 0; i < length; ++i) {
      if (::hit(ptr[i], r, t_min, closest_so_far, tmp_rec)) {
        hit_anything = true;
        closest_so_far = tmp_rec.t;
        rec = tmp_rec;
      }
    }

    return hit_anything;
  }

public:
  hittable *ptr;
  size_t length;
};
