#include "interval.h"
#include "rt_utility.h"

const interval interval::empty = interval(infinity, -infinity);
const interval interval::universe = interval(-infinity, infinity);

interval interval::expand(double delta) const {
  double padding = delta / 2;
  return interval(iMin - padding, iMax + padding);
};

double interval::clamp(double x) const {
  return (x < 0.0 ? 0.0 : fmin(x, 1.0));
}
