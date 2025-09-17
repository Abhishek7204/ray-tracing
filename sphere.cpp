#include "sphere.h"
#include "interval.h"
#include "ray.h"

bool sphere::isHit(const ray &r, interval ray_t, hitRecord &record) const {
  // (C - P).(C - P) = (Cx - x)^2 + (Cy - y)^2 + (Cz - z)^2 = r^2
  // (C - (Q + dt)).(C - (Q + dt)) = r^2
  // t^2 d.d - 2td.(C-Q)+(C-Q).(C-Q) - r^2 = 0
  // a = d.d , b = -2d.(C-Q) , c = (C-Q).(C-Q) - r^2
  // discriminant = sqrt(b^2 - 4ac) no of roots is no of points touching
  double a = dotProduct(r.direction(), r.direction());
  double b = -2 * dotProduct(r.direction(), center - r.origin());
  double c =
      dotProduct(center - r.origin(), center - r.origin()) - radius * radius;
  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0)
    return false;
  double discriminantSqrt = sqrt(discriminant);
  double root = (-b - discriminantSqrt) / (2.0 * a);
  if (root < ray_t.iMin || root > ray_t.iMax) {
    root = (-b + discriminantSqrt) / (2.0 * a);
    if (root < ray_t.iMin || root > ray_t.iMax)
      return false;
  }

  record.t = root;
  record.contactPoint = r.at(root);
  record.hitNormal = (record.contactPoint - center) / radius;
  return true;
};
