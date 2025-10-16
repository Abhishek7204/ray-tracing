#include "quadrilateral.h"
#include "ray.h"
#include "vect.h"

quadrilateral::quadrilateral(const point3 &Q, const vect &u, const vect &v,
                             shared_ptr<material> mat)
    : Q(Q), u(u), v(v), mat(mat) {
  auto n = crossProduct(u, v);
  normal = unitVector(n);
  D = dotProduct(normal, Q);
  setBoundingBox();
}

void quadrilateral::setBoundingBox() {
  auto bboxDiagnol1 = aabb(Q, Q + u + v);
  auto bboxDiagnol2 = aabb(Q + u, Q + v);
  bbox = aabb(bboxDiagnol1, bboxDiagnol2);
}

bool quadrilateral::isHit(const ray &r, interval ray_t,
                          hitRecord &record) const {
  auto denominator = dotProduct(normal, r.direction());
  if (denominator == 0)
    return false;

  auto t = (D - dotProduct(normal, r.origin())) / denominator;
  if (!ray_t.contains(t))
    return false;

  record.t = t;
  record.contactPoint = r.at(t);
  record.hitMaterial = mat;
  record.hitNormal = normal * (dotProduct(normal, r.direction()) > 0 ? -1 : 1);
  return true;
}
