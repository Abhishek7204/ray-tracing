#include "scene_object.h"
#include "ray.h"
#include "rt_utility.h"
#include "vect.h"

bool translate::isHit(const ray &r, interval ray_t, hitRecord &record) const {
  ray offsetRay(r.origin() - offset, r.direction(), r.getTime());

  if (!object->isHit(offsetRay, ray_t, record))
    return false;

  record.contactPoint += offset;
  return true;
}

rotateY::rotateY(shared_ptr<sceneObject> object, double angle)
    : object(object) {
  auto radians = degreeToRadian(angle);
  sinTheta = std::sin(radians);
  cosTheta = std::cos(radians);
  bbox = object->boundingBox();

  point3 min(infinity, infinity, infinity);
  point3 max(-infinity, -infinity, -infinity);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        auto x = i * bbox[0].iMax + (1 - i) * bbox[0].iMin;
        auto y = j * bbox[1].iMax + (1 - j) * bbox[1].iMin;
        auto z = k * bbox[2].iMax + (1 - k) * bbox[2].iMin;

        auto newx = cosTheta * x + sinTheta * z;
        auto newz = -sinTheta * x + cosTheta * z;

        vect tester(newx, y, newz);

        for (int c = 0; c < 3; c++) {
          min[c] = std::fmin(min[c], tester[c]);
          max[c] = std::fmax(max[c], tester[c]);
        }
      }
    }
  }

  bbox = aabb(min, max);
}

bool rotateY::isHit(const ray &r, interval ray_t, hitRecord &record) const {
  auto origin = point3(
      (cosTheta * r.origin().x()) - (sinTheta * r.origin().z()), r.origin().y(),
      (sinTheta * r.origin().x()) + (cosTheta * r.origin().z()));

  auto direction =
      vect((cosTheta * r.direction().x()) - (sinTheta * r.direction().z()),
           r.direction().y(),
           (sinTheta * r.direction().x()) + (cosTheta * r.direction().z()));

  ray rotated_r(origin, direction, r.getTime());

  // Determine whether an intersection exists in object space (and if so,
  // where).

  if (!object->isHit(rotated_r, ray_t, record))
    return false;

  // Transform the intersection from object space back to world space.

  record.contactPoint = point3((cosTheta * record.contactPoint.x()) +
                                   (sinTheta * record.contactPoint.z()),
                               record.contactPoint.y(),
                               (-sinTheta * record.contactPoint.x()) +
                                   (cosTheta * record.contactPoint.z()));

  record.hitNormal = vect(
      (cosTheta * record.hitNormal.x()) + (sinTheta * record.hitNormal.z()),
      record.hitNormal.y(),
      (-sinTheta * record.hitNormal.x()) + (cosTheta * record.hitNormal.z()));

  return true;
};
