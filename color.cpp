#include "color.h"

void printColor(ostream &out, const color &pixel) {
  auto r = linearToGamma(pixel.x());
  auto g = linearToGamma(pixel.y());
  auto b = linearToGamma(pixel.z());
  int rByte = int(255.999 * r);
  int gByte = int(255.999 * g);
  int bByte = int(255.999 * b);

  out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}
