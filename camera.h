#ifndef CAMERA_H
#define CAMERA_H

#include "scene_object_list.h"

class camera {
  int imgHeight;
  point3 cameraCenter;
  vect vpHorizontalDel, vpVerticalDel;
  point3 vpFirstPixel;

  void initialize();

public:
  void render(const sceneObjectList &world);
  double aspectRatio = 16.0 / 9.0;
  int imgWidth = 400;
  int sampleCount = 5;
  int sampleDepth = 10;
};
#endif // !CAMERA_H
