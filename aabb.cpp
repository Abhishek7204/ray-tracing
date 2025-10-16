#include "aabb.h"
#include "ray.h"

aabb::aabb(const point3 &a, const point3 &b) {
  for (int i = 0; i < 3; i++)
    axes[i] = interval(min(a[i], b[i]), max(a[i], b[i]));
}

aabb::aabb(const aabb &boxA, const aabb &boxB) {
  for (int i = 0; i < 3; i++) {
    axes[i].iMin = min(boxA[i].iMin, boxB[i].iMin);
    axes[i].iMax = max(boxA[i].iMax, boxB[i].iMax);
  }
};

bool aabb::hit(const ray &r, interval ray_t) const {
  const point3 &rayOrg = r.origin();
  const vect &rayDir = r.direction();

  for (int i = 0; i < 3; i++) {
    double t0 = (axes[i].iMin - rayOrg[i]) / rayDir[i];
    double t1 = (axes[i].iMax - rayOrg[i]) / rayDir[i];

    if (t1 < t0)
      swap(t1, t0);

    ray_t.iMin = max(ray_t.iMin, t0);
    ray_t.iMax = min(ray_t.iMax, t1);

    if (ray_t.iMax <= ray_t.iMin)
      return false;
  }
  return true;
}

int aabb::longestAxis() {
  int maxSize = -1, maxAxis = 0;
  for (int i = 0; i < 3; i++) {
    if (axes[i].size() > maxSize) {
      maxSize = axes[i].size();
      maxAxis = i;
    }
  }
  return maxAxis;
}

void aabb::paddingMinimum() {
  double delta = 0.0001;
  for (int i = 0; i < 3; i++)
    if (axes[i].size() < delta)
      axes[i] = axes[i].expand(delta);
}

const aabb aabb::empty =
    aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe =
    aabb(interval::universe, interval::universe, interval::universe);
