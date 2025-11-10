#include "scene_object.h"
#include "ray.h"

bool translate::isHit(const ray &r, interval ray_t, hitRecord &record) const {
  ray offsetRay(r.origin() - offset, r.direction(), r.getTime());

  if (!object->isHit(offsetRay, ray_t, record))
    return false;

  record.contactPoint += offset;
  return true;
}
