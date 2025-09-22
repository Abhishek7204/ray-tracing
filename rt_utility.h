#ifndef RT_UTILITY_H
#define RT_UTILITY_H

#include "interval.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>

class interval;

using std::make_shared;
using std::shared_ptr;

const double pi = 3.1415926535897932385;

inline double degreeToRadian(double degrees) { return degrees * pi / 180.0; }

inline double randomDouble() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double randomDouble(double lower, double upper) {
  return lower + (upper - lower) * randomDouble();
}

#endif // !RT_UTILITY_H
