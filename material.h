#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"

class material {
public:
  virtual bool scatter(const ray &r, const hitRecord &record,
                       color &attenuation, ray &scattered) const;
};

class lambertian : public material {
  color albedo;

public:
  lambertian(const color &albedo) : albedo(albedo){};

  bool scatter(const ray &r, const hitRecord &record, color &attenuation,
               ray &scattered) const override;
};

class metal : public material {
  color albedo;
  double fuzzFactor = 1;

public:
  metal(const color &albedo, const double &fuzzFactor)
      : albedo(albedo), fuzzFactor(min(fuzzFactor, 1.0)){};

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
#endif // !MATERIAL_H
