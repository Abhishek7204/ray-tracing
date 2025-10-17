#include "quadrilateral.h"
#include "interval.h"
#include "ray.h"
#include "vect.h"

quadrilateral::quadrilateral(const point3 &Q, const vect &u, const vect &v,
                             shared_ptr<material> mat)
    : Q(Q), u(u), v(v), mat(mat) {
  auto n = crossProduct(u, v);
  normal = unitVector(n);
  D = dotProduct(normal, Q);
  W = n / dotProduct(n, n);

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
  if (fabs(denominator) < 1e-8)
    return false;

  auto t = (D - dotProduct(normal, r.origin())) / denominator;
  if (!ray_t.contains(t))
    return false;

  point3 intersection = r.at(t);
  vect p = intersection - Q;
  auto alpha = dotProduct(W, crossProduct(p, v));
  auto beta = dotProduct(W, crossProduct(u, p));

  if (!isInside(alpha, beta, record))
    return false;
  record.t = t;
  record.contactPoint = r.at(t);
  record.hitMaterial = mat;
  record.hitNormal = normal * (dotProduct(normal, r.direction()) > 0 ? -1 : 1);
  return true;
}
bool quadrilateral::isInside(double alpha, double beta,
                             hitRecord &record) const {
  interval unit = interval(0, 1);

  if (!unit.contains(alpha) || !unit.contains(beta))
    return false;

  record.u = alpha;
  record.v = beta;
  return true;
}
