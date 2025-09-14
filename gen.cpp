#include "color.h"
#include "ray.h"
#include "vect.h"
#include <algorithm>
#include <iostream>

using namespace std;

bool hasHitSphere(const ray &r, const point3 &center, double radius) {
  // (C - P).(C - P) = (Cx - x)^2 + (Cy - y)^2 + (Cz - z)^2 = r^2
  // (C - (Q + dt)).(C - (Q + dt)) = r^2
  // t^2 d.d - 2td.(C-Q)+(C-Q).(C-Q) - r^2 = 0
  // a = d.d , b = -2d.(C-Q) , c = (C-Q).(C-Q) - r^2
  // discriminant = sqrt(b^2 - 4ac) no of roots is no of points touching sphere
  double a = dotProduct(r.direction(), r.direction());
  double b = -2 * dotProduct(r.direction(), center - r.origin());
  double c =
      dotProduct(center - r.origin(), center - r.origin()) - radius * radius;
  double discriminant = b * b - 4 * a * c;
  return discriminant >= 0;
}

int main() {
  cout << "P3" << '\n';

  double aspectRatio = 16.0 / 9.0;
  int imgWidth = 400;
  int imgHeight = max(1, static_cast<int>(imgWidth / aspectRatio));

  cout << imgWidth << " " << imgHeight << "\n255\n";

  point3 cameraCenter = point3(0, 0, 0);
  double focalLen = 1.0;
  double vpHeight = 2.0;
  double vpWidth = vpHeight * (static_cast<double>(imgWidth) / imgHeight);

  vect vpHorizontal = vect(vpWidth, 0, 0);
  vect vpVertical = vect(0, -vpHeight, 0);

  vect vpHorizontalDel = vpHorizontal / imgWidth;
  vect vpVerticalDel = vpVertical / imgHeight;

  point3 vpCorner =
      cameraCenter + vect(0, 0, -focalLen) - vpHorizontal / 2 - vpVertical / 2;
  point3 vpFirstPixel = vpCorner + (vpHorizontalDel + vpVerticalDel) / 2;

  for (int h = 0; h < imgHeight; h++) {
    clog << "\rScanlines remaining: " << (imgHeight - h) << ' ' << flush;
    for (int w = 0; w < imgWidth; w++) {
      point3 pixelCenter =
          vpFirstPixel + w * vpHorizontalDel + h * vpVerticalDel;
      point3 rayDirection = pixelCenter - cameraCenter;
      ray r(cameraCenter, rayDirection);
      color pixelColor =
          (hasHitSphere(r, point3(0, 0, -1), 0.5) ? color(0, 0, 0)
                                                  : rayColor(r));
      printColor(cout, pixelColor);
    }
    cout << '\n';
  }
  clog << "\rDone.                    \n";
}
