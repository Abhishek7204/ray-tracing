#include "camera.h"
#include "material.h"
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
  cam.sampleDepth = 10;

  // Materials
  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left = make_shared<dielectric>(1.50);
  auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
  auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
  // World
  sceneObjectList world;
  world.add(
      make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
  world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

  auto start = chrono::high_resolution_clock::now();
  cam.render(world);
  auto stop = chrono::high_resolution_clock::now();
  chrono::duration<double> duration = stop - start;
  clog << "Execution time: " << duration.count() << " seconds" << endl;
}
