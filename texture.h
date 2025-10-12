#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "perlin.h"
#include "rtimage.h"
#include "vect.h"
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

class imageTexture : public texture {
  rtwimage img;

public:
  imageTexture(const char *file) : img(file) {}

  color value(double u, double v, const point3 &p) const override {
    if (img.height() <= 0)
      return color(0, 1, 1);

    u = interval(0, 1).clamp(u);
    v = 1.0 - interval(0, 1).clamp(v);

    auto i = int(u * img.width());
    auto j = int(v * img.height());
    auto pixel = img.pixel_data(i, j);

    auto colorScale = 1.0 / 255.0;
    return color(colorScale * pixel[0], colorScale * pixel[1],
                 colorScale * pixel[2]);
  }
};

class noiseTexture : public texture {
  perlinNoise seed;

public:
  noiseTexture() {}

  color value(double u, double v, const point3 &p) const override {
    return color(1, 1, 1) * seed.noise(p);
  }
};
#endif // !TEXTURE_H
