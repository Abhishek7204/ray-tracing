#include "material.h"

bool material::scatter(const ray &r, const hitRecord &record,
                       color &attenuation, ray &scattered) const {
  return false;
}

bool lambertian::scatter(const ray &r, const hitRecord &record,
                         color &attenuation, ray &scattered) const {
  vect scatterDirection = record.hitNormal + randomUnitVector();
  if (scatterDirection.nearZero())
    scatterDirection = record.hitNormal;
  scattered = ray(record.contactPoint, scatterDirection);
  attenuation = albedo;
  return true;
};

bool metal::scatter(const ray &r, const hitRecord &record, color &attenuation,
                    ray &scattered) const {
  vect reflected = reflection(record.hitNormal, r.direction());
  vect fuzzyReflected = unitVector(reflected) + fuzzFactor * randomUnitVector();
  scattered = ray(record.contactPoint, fuzzyReflected);
  attenuation = albedo;
  return (dotProduct(record.hitNormal, fuzzyReflected) > 0);
};

bool dielectric::scatter(const ray &r, const hitRecord &record,
                         color &attenuation, ray &scattered) const {
  attenuation = color(1, 1, 1);
  bool outOfSphere = dotProduct(record.hitNormal, r.direction()) > 0;
  double ri = (!outOfSphere ? (1.0 / refractiveIndex) : refractiveIndex);

  vect normal = (outOfSphere ? -record.hitNormal : record.hitNormal);
  vect refracted = refract(normal, unitVector(r.direction()), ri);

  scattered = ray(record.contactPoint, refracted);
  return true;
}
