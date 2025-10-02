#include "interval.h"

const interval interval::empty = interval(infinity, -infinity);
const interval interval::universe = interval(-infinity, infinity);

interval interval::expand(double delta) const {
  double padding = delta / 2;
  return interval(iMin - padding, iMax + padding);
};
