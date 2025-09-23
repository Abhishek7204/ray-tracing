#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "ray.h"
#include "scene_object.h"
#include "vect.h"

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

class metal : public material {
  color albedo;
  double fuzzFactor = 1;

public:
  metal(const color &albedo, const double &fuzzFactor)
      : albedo(albedo), fuzzFactor(max(fuzzFactor, 1.0)){};

  bool scatter(const ray &r, const hitRecord &record, color &attenuation,
               ray &scattered) const override {
    vect reflected = reflection(record.hitNormal, r.direction());
    vect fuzzyReflected =
        unitVector(reflected) + fuzzFactor * randomUnitVector();
    scattered = ray(record.contactPoint, fuzzyReflected);
    attenuation = albedo;
    return (dotProduct(record.hitNormal, fuzzyReflected) > 0);
  };
};

class dielectric : public material {
  double reflectiveIndex;

public:
  dielectric(double reflectiveIndex) : reflectiveIndex(reflectiveIndex) {}
  bool scatter(const ray &r, const hitRecord &record, color &attenuation,
               ray &scattered) const override {
    attenuation = color(1, 1, 1);
    bool outOfSphere = dotProduct(record.hitNormal, r.direction()) > 0;
    double ri = (!outOfSphere ? (1.0 / reflectiveIndex) : reflectiveIndex);

    vect normal = (outOfSphere ? -record.hitNormal : record.hitNormal);
    vect refracted = refract(normal, unitVector(r.direction()), ri);

    scattered = ray(record.contactPoint, refracted);
    return true;
  }
};
#endif // !MATERIAL_H
