#ifndef CAMERA_H
#define CAMERA_H

#include "rt_utility.h"
#include "scene_object_list.h"

class camera {
  int imgHeight;
  point3 cameraCenter;
  vect vpHorizontalDel, vpVerticalDel;
  point3 vpFirstPixel;
  vect basisU, basisV, basisW;
  vect defocusDiskU, defocusDiskV;

  void initialize();
  point3 defocusDiskSample() const;

public:
  void render(const sceneObjectList &world);
  double aspectRatio = 16.0 / 9.0;
  int imgWidth = 400;
  int sampleCount = 5;
  int sampleDepth = 10;
  double verticalFOV = 20;
  point3 lookFrom = point3(0, 0, 0);
  point3 lookAt = point3(0, 0, -1);
  point3 verticalUp = point3(0, 1, 0);

  double defocusAngle = 0;
  double focusDist = 10;
};
#endif // !CAMERA_H
