#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

const double infinity = std::numeric_limits<double>::infinity();
class interval {

public:
  double iMin, iMax;
  interval() : iMin(+infinity), iMax(-infinity) {}

  interval(double iMin, double iMax) : iMin(iMin), iMax(iMax) {}

  double size() const { return iMax - iMin; }

  bool contains(double x) const { return iMin <= x && iMax >= x; }

  bool surrounds(double x) const { return iMin < x && x < iMax; }

  static const interval empty, universe;
};

#endif
