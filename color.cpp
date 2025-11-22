#include "color.h"

void printColor(ostream &out, const color &pixel) {
  auto r = linearToGamma(pixel.x());
  auto g = linearToGamma(pixel.y());
  auto b = linearToGamma(pixel.z());

  static const interval intensity(0.000, 0.999);
  int rByte = int(255.999 * intensity.clamp(r));
  int gByte = int(255.999 * intensity.clamp(g));
  int bByte = int(255.999 * intensity.clamp(b));

  out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}
