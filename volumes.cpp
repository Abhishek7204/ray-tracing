#include "volumes.h"
#include "rt_utility.h"
#include "scene_object.h"

bool volumes::isHit(const ray &r, interval ray_t, hitRecord &record) const {
  hitRecord rec1, rec2;

  if (!boundary->isHit(r, interval::universe, rec1))
    return false;

  if (!boundary->isHit(r, interval(rec1.t + 0.0001, infinity), rec2))
    return false;

  if (rec1.t < ray_t.iMin)
    rec1.t = ray_t.iMin;
  if (rec2.t > ray_t.iMax)
    rec2.t = ray_t.iMax;

  if (rec1.t >= rec2.t)
    return false;

  if (rec1.t < 0)
    rec1.t = 0;

  auto ray_length = r.direction().len();
  auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
  auto hit_distance = (-1 / density) * std::log(randomDouble());

  if (hit_distance > distance_inside_boundary)
    return false;

  record.t = rec1.t + hit_distance / ray_length;
  record.contactPoint = r.at(record.t);

  record.hitNormal = vect(1, 0, 0); // arbitrary
  record.hitMaterial = phaseFunc;

  return true;
}
