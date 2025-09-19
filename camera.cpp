#include "camera.h"
#include "ray.h"

void camera::initialize() {
  imgHeight = max(1, static_cast<int>(imgWidth / aspectRatio));

  // Camera and Viewport
  cameraCenter = point3(0, 0, 0);
  double focalLen = 1.0;
  double vpHeight = 2.0;
  double vpWidth = vpHeight * (static_cast<double>(imgWidth) / imgHeight);

  vect vpHorizontal = vect(vpWidth, 0, 0);
  vect vpVertical = vect(0, -vpHeight, 0);

  vpHorizontalDel = vpHorizontal / imgWidth;
  vpVerticalDel = vpVertical / imgHeight;

  point3 vpCorner =
      cameraCenter + vect(0, 0, -focalLen) - vpHorizontal / 2 - vpVertical / 2;
  vpFirstPixel = vpCorner + (vpHorizontalDel + vpVerticalDel) / 2;
}

void camera::render(const sceneObjectList &world) {
  initialize();
  cout << "P3" << '\n';
  cout << imgWidth << " " << imgHeight << "\n255\n";
  for (int h = 0; h < imgHeight; h++) {
    clog << "\rScanlines remaining: " << (imgHeight - h) << ' ' << flush;
    for (int w = 0; w < imgWidth; w++) {
      point3 pixelCenter =
          vpFirstPixel + w * vpHorizontalDel + h * vpVerticalDel;
      color totalColor;
      for (int it = 0; it < sampleCount; it++) {
        vect xDisplacement(vpHorizontalDel.x() * (randomDouble() - 0.5), 0, 0);
        vect yDisplacement(0, vpHorizontalDel.y() * (randomDouble() - 0.5), 0);
        point3 samplePixel = pixelCenter + xDisplacement + yDisplacement;
        point3 rayDirection = samplePixel - cameraCenter;
        ray r(cameraCenter, rayDirection);
        totalColor += rayColor(r, sampleDepth, world);
      }
      printColor(cout, totalColor / sampleCount);
    }
    cout << '\n';
  }
  clog << "\rDone.                    \n";
}
