#include "color.h"

void printColor(ostream &out, const color &pixel) {
  auto r = pixel[0];
  auto g = pixel[1];
  auto b = pixel[2];
  int rByte = int(255.999 * r);
  int gByte = int(255.999 * g);
  int bByte = int(255.999 * b);

  out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}
