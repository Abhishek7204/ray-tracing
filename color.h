#ifndef COLOR_H
#define COLOR_H

#include "vect.h"

using color = vect;

inline double linearToGamma(double linear) { return sqrt(max(0.0, linear)); }

void printColor(ostream &out, const color &pixel);

#endif // !CAMERA_H
