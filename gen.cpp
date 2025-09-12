#include "color.h"
#include "vect.h"
#include <iostream>

using namespace std;

int main() {
  cout << "P3" << '\n';

  int imgWidth = 256;
  int imgHeight = 256;

  cout << imgWidth << " " << imgHeight << "\n255\n";

  for (int h = 0; h < imgHeight; h++) {
    clog << "\rScanlines remaining: " << (imgHeight - h) << ' ' << flush;
    for (int w = 0; w < imgWidth; w++) {
      auto pixel_color =
          color(double(h) / (imgHeight - 1), double(w) / (imgWidth - 1), 0);
      print_color(cout, pixel_color);
    }
    cout << '\n';
  }
  clog << "\rDone.                    \n";
}
