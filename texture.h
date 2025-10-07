#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include <cmath>
#include <memory>

class texture {
public:
  virtual ~texture() = default;

  virtual color value(double u, double v, const point3 &p) const = 0;
};

class solidColor : public texture {
  color albedo;

public:
  solidColor(const color &albedo) : albedo(albedo) {}

  solidColor(double red, double green, double blue)
      : solidColor(color(red, green, blue)) {}

  color value(double u, double v, const point3 &p) const override {
    return albedo;
  }
};

class checkerTexture : public texture {
  double scale;
  shared_ptr<texture> even, odd;

public:
  checkerTexture(double scale, shared_ptr<texture> even,
                 shared_ptr<texture> odd)
      : scale(scale), even(even), odd(odd) {}

  checkerTexture(double scale, const color &c1, const color &c2)
      : checkerTexture(scale, make_shared<solidColor>(c1),
                       make_shared<solidColor>(c2)) {}

  color value(double u, double v, const point3 &p) const override {
    int x = floor(p.x() / scale);
    int y = floor(p.y() / scale);
    int z = floor(p.z() / scale);

    return ((x + y + z) & 1 ? odd->value(u, v, p) : even->value(u, v, p));
  }
};
#endif // !TEXTURE_H
