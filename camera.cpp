#include "camera.h"
#include "ray.h"
#include "rt_utility.h"
#include "vect.h"

void camera::initialize() {
  imgHeight = max(1, static_cast<int>(imgWidth / aspectRatio));

  // Basis
  basisW = unitVector(lookFrom - lookAt);
  basisU = unitVector(crossProduct(verticalUp, basisW));
  basisV = crossProduct(basisW, basisU);

  // Camera and Viewport
  cameraCenter = lookFrom;
  double heightRatio = tan(degreeToRadian(verticalFOV) / 2);
  double vpHeight = 2.0 * heightRatio * focusDist;
  double vpWidth = vpHeight * (static_cast<double>(imgWidth) / imgHeight);

  vect vpHorizontal = vpWidth * basisU;
  vect vpVertical = -vpHeight * basisV;

  vpHorizontalDel = vpHorizontal / imgWidth;
  vpVerticalDel = vpVertical / imgHeight;

  point3 vpCorner =
      cameraCenter + (basisW * -focusDist) - vpHorizontal / 2 - vpVertical / 2;
  vpFirstPixel = vpCorner + (vpHorizontalDel + vpVerticalDel) / 2;

  double defocusRadius = focusDist * tan(degreeToRadian(defocusAngle / 2));
  defocusDiskU = basisU * defocusRadius;
  defocusDiskV = basisV * defocusRadius;
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
        point3 rayOrigin =
            (defocusAngle > 0 ? defocusDiskSample() : cameraCenter);
        point3 rayDirection = samplePixel - rayOrigin;
        ray r(rayOrigin, rayDirection);
        totalColor += rayColor(r, sampleDepth, world);
      }
      printColor(cout, totalColor / sampleCount);
    }
    cout << '\n';
  }
  clog << "\rDone.                    \n";
}

point3 camera::defocusDiskSample() const {
  vect p = randomOnUnitDisk();
  return cameraCenter + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
}
