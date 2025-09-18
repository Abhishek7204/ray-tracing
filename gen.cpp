#include "camera.h"
#include "rt_utility.h"
#include "scene_object_list.h"
#include "sphere.h"
#include <chrono>

using namespace std;

int main() {

  camera cam;
  cam.aspectRatio = 16.0 / 9.0;
  cam.imgWidth = 400;
  cam.sampleCount = 25;

  // World
  sceneObjectList world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  auto start = chrono::high_resolution_clock::now();
  cam.render(world);
  auto stop = chrono::high_resolution_clock::now();
  chrono::duration<double> duration = stop - start;
  clog << "Execution time: " << duration.count() << " seconds" << endl;
}
