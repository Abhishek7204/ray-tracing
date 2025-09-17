#ifndef RT_UTILITY_H
#define RT_UTILITY_H

#include "interval.h"
#include <cmath>
#include <iostream>
#include <memory>

class interval;

using std::make_shared;
using std::shared_ptr;

const double pi = 3.1415926535897932385;

inline double degreeToRadian(double degrees) { return degrees * pi / 180.0; }

#endif
