#include "sphere.h"
#include "interval.h"
#include "ray.h"
#include <cmath>

sphere::sphere(const point3 &center, double radius, shared_ptr<material> mat)
    : centerLocations(center, vect(0, 0, 0)), radius(max(0.0, radius)),
      mat(mat) {
  vect rVec = vect(1, 1, 1) * radius;
  bbox = aabb(center - rVec, center + rVec);
}
sphere::sphere(const point3 &center1, const point3 &center2, double radius,
               shared_ptr<material> mat)
    : centerLocations(center1, center2 - center1), radius(max(0.0, radius)),
      mat(mat) {
  vect rVec = vect(1, 1, 1) * radius;
  aabb box1 = aabb(center1 - rVec, center1 + rVec);
  aabb box2 = aabb(center2 - rVec, center2 + rVec);
  bbox = aabb(box1, box2);
}
bool sphere::isHit(const ray &r, interval ray_t, hitRecord &record) const {
  // (C - P).(C - P) = (Cx - x)^2 + (Cy - y)^2 + (Cz - z)^2 = r^2
  // (C - (Q + dt)).(C - (Q + dt)) = r^2
  // t^2 d.d - 2td.(C-Q)+(C-Q).(C-Q) - r^2 = 0
  // a = d.d , b = -2d.(C-Q) , c = (C-Q).(C-Q) - r^2
  // discriminant = sqrt(b^2 - 4ac) no of roots is no of points touching
  point3 center = centerLocations.at(r.getTime());
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
  record.hitMaterial = mat;
  getUV(record.hitNormal, record);
  return true;
};

void sphere::getUV(const point3 &p, hitRecord &record) const {
  double theta = acos(-p.y());
  double phi = atan2(-p.z(), p.x()) + pi;

  record.u = phi / (2 * pi);
  record.v = theta / pi;
};
