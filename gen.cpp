#include "camera.h"
#include "material.h"
#include "rt_utility.h"
#include "scene_object_list.h"
#include "sphere.h"

using namespace std;

int main() {

  // Camera
  camera cam;
  cam.aspectRatio = 16.0 / 9.0;
  cam.imgWidth = 1200;
  cam.sampleCount = 20;
  cam.sampleDepth = 5;

  cam.verticalFOV = 20;
  cam.lookFrom = point3(13, 2, 3);
  cam.lookAt = point3(0, 0, 0);
  cam.verticalUp = vect(0, 1, 0);

  cam.defocusAngle = 0.6;
  cam.focusDist = 10.0;

  // Materials
  sceneObjectList world;
  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = randomDouble();
      point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

      if ((center - point3(4, 0.2, 0)).len() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = randomDouble(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  cam.render(world);
}
