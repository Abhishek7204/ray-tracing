#include "color.h"

void print_color(ostream &out, const color &pixel) {
  auto r = pixel[0];
  auto g = pixel[1];
  auto b = pixel[2];
  int rbyte = int(255.999 * r);
  int gbyte = int(255.999 * g);
  int bbyte = int(255.999 * b);

  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
