#include "camera.h"
#include "ray.h"
#include "rt_utility.h"
#include "vect.h"
#include <omp.h>

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
  std::cout << "P3\n" << imgWidth << " " << imgHeight << "\n255\n";

  std::vector<std::vector<color>> framebuffer(imgHeight,
                                              std::vector<color>(imgWidth));

  auto start = std::chrono::high_resolution_clock::now();

  int scanLinesLeft = imgHeight;
#pragma omp parallel for schedule(dynamic)
  for (int h = 0; h < imgHeight; h++) {
    for (int w = 0; w < imgWidth; w++) {
      point3 pixelCenter =
          vpFirstPixel + w * vpHorizontalDel + h * vpVerticalDel;
      color totalColor(0, 0, 0);
      for (int it = 0; it < sampleCount; it++) {
        ray r = getRay(pixelCenter);
        totalColor += rayColor(r, sampleDepth, world);
      }
      framebuffer[h][w] = totalColor / sampleCount;
    }
#pragma omp critical
    {
      auto curr = chrono::high_resolution_clock::now();
      chrono::duration<double> duration = curr - start;
      scanLinesLeft--;
      double timePerLine = duration.count() / (imgHeight - scanLinesLeft);
      clog << "\rScanlines Left : " << scanLinesLeft
           << " Execution Time Left : " << timePerLine * scanLinesLeft
           << " seconds  " << flush;
    }
  }

  for (int h = 0; h < imgHeight; h++) {
    for (int w = 0; w < imgWidth; w++) {
      printColor(std::cout, framebuffer[h][w]);
    }
    std::cout << '\n';
  }

  std::clog << "\rDone.                                                        "
               "    \n";

  auto stop = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = stop - start;
  std::clog << "Execution time: " << duration.count() << " seconds"
            << std::endl;
}

color camera::rayColor(const ray &r, int depthLeft,
                       const sceneObjectList &world) {
  if (!depthLeft)
    return color();
  hitRecord record;
  if (world.isHit(r, interval(0.001, infinity), record)) {
    color attenuation;
    ray scattered;
    color emission =
        record.hitMaterial->emitted(record.u, record.v, record.contactPoint);
    if (record.hitMaterial->scatter(r, record, attenuation, scattered))
      return emission + attenuation * rayColor(scattered, depthLeft - 1, world);
    return emission;
  } else {
    return backGround;
  }

  vect unitDirection = unitVector(r.direction());
  auto a = 0.5 * (unitDirection.y() + 1.0);
  return (1 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
}

ray camera::getRay(point3 pixelCenter) const {
  vect xDisplacement(vpHorizontalDel.x() * (randomDouble() - 0.5), 0, 0);
  vect yDisplacement(0, vpHorizontalDel.y() * (randomDouble() - 0.5), 0);
  point3 samplePixel = pixelCenter + xDisplacement + yDisplacement;
  point3 rayOrigin = (defocusAngle > 0 ? defocusDiskSample() : cameraCenter);
  point3 rayDirection = samplePixel - rayOrigin;
  double rayTime = randomDouble();
  return ray(rayOrigin, rayDirection, rayTime);
}

point3 camera::defocusDiskSample() const {
  vect p = randomOnUnitDisk();
  return cameraCenter + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
}
