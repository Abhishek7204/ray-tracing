#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "ray.h"
#include "texture.h"
#include <memory>

class material {
public:
  virtual bool scatter(const ray &r, const hitRecord &record,
                       color &attenuation, ray &scattered) const;

  virtual color emitted(double u, double v, const point3 &p) const {
    return color(0, 0, 0);
  };
};

class lambertian : public material {
  shared_ptr<texture> tex;

public:
  lambertian(const color &albedo) : tex(make_shared<solidColor>(albedo)) {};

  lambertian(shared_ptr<texture> tex) : tex(tex) {}

  bool scatter(const ray &r, const hitRecord &record, color &attenuation,
               ray &scattered) const override;
};

class metal : public material {
  color albedo;
  double fuzzFactor = 1;

public:
  metal(const color &albedo, const double &fuzzFactor)
      : albedo(albedo), fuzzFactor(min(fuzzFactor, 1.0)) {};

  bool scatter(const ray &r, const hitRecord &record, color &attenuation,
               ray &scattered) const override;
};

class dielectric : public material {
  double refractiveIndex;

public:
  dielectric(double refractriveIndex) : refractiveIndex(refractriveIndex) {}
  bool scatter(const ray &r, const hitRecord &record, color &attenuation,
               ray &scattered) const override;
};

class diffuseLight : public material {
  shared_ptr<texture> tex;

public:
  diffuseLight(shared_ptr<texture> tex) : tex(tex) {}
  diffuseLight(const color &emitColor)
      : tex(make_shared<solidColor>(emitColor)) {}

  color emitted(double u, double v, const point3 &p) const override {
    return tex->value(u, v, p);
  }
};
#endif // !MATERIAL_H
