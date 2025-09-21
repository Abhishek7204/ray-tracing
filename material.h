#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "ray.h"
#include "scene_object.h"

class material {
public:
  virtual bool scatter(const ray &r, const hitRecord &record,
                       color &attenuation, ray &scattered) const {
    return false;
  }
};

class lambertian : public material {
  color albedo;

public:
  lambertian(const color &albedo) : albedo(albedo){};

  bool scatter(const ray &r, const hitRecord &record, color &attenuation,
               ray &scattered) const override {
    vect scatterDirection = record.hitNormal + randomUnitVector();
    if (scatterDirection.nearZero())
      scatterDirection = record.hitNormal;
    scattered = ray(record.contactPoint, scatterDirection);
    attenuation = albedo;
    return true;
  };
};
#endif // !MATERIAL_H
