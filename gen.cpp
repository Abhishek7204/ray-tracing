#include "bvh.h"
#include "camera.h"
#include "material.h"
#include "rt_utility.h"
#include "scene_object_list.h"
#include "sphere.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

using namespace std;

void bouncingSpheres() {
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
          auto center2 = center + vect(0, randomDouble(0, .5), 0);
          world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
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

  camera cam;

  cam.aspectRatio = 16.0 / 9.0;
  cam.imgWidth = 1200;
  cam.sampleCount = 100;
  cam.sampleDepth = 50;

  cam.verticalFOV = 20;
  cam.lookFrom = point3(13, 2, 3);
  cam.lookAt = point3(0, 0, 0);
  cam.verticalUp = vect(0, 1, 0);

  cam.defocusAngle = 0.6;
  cam.focusDist = 10.0;

  world = sceneObjectList(make_shared<bvh>(world));
  cam.render(world);
}

void checkedSpheres() {
  sceneObjectList world;

  auto checker =
      make_shared<checkerTexture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

  world.add(make_shared<sphere>(point3(0, -10, 0), 10,
                                make_shared<lambertian>(checker)));
  world.add(make_shared<sphere>(point3(0, 10, 0), 10,
                                make_shared<lambertian>(checker)));

  camera cam;

  cam.aspectRatio = 16.0 / 9.0;
  cam.imgWidth = 400;
  cam.sampleCount = 100;
  cam.sampleDepth = 50;

  cam.verticalFOV = 20;
  cam.lookFrom = point3(13, 2, 3);
  cam.lookAt = point3(0, 0, 0);
  cam.verticalUp = vect(0, 1, 0);

  cam.defocusAngle = 0;

  cam.render(world);
}

void threeSpheres() {
  camera cam;
  cam.aspectRatio = 16.0 / 9.0;
  cam.imgWidth = 400;
  cam.sampleCount = 25;
  cam.sampleDepth = 10;
  cam.lookFrom = point3(-2, 2, 1);
  cam.lookAt = point3(0, 0, -1);
  cam.verticalUp = vect(0, 1, 0);

  // Materials
  auto checker =
      make_shared<checkerTexture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
  auto material_checked = make_shared<lambertian>(checker);
  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left = make_shared<dielectric>(1.50);
  auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
  auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

  // World
  sceneObjectList world;
  world.add(
      make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_checked));
  world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
  world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

  world = sceneObjectList(make_shared<bvh>(world));
  cam.render(world);
}

void earth() {
  auto earth_texture = make_shared<imageTexture>("earthmap.jpg");
  auto earth_surface = make_shared<lambertian>(earth_texture);
  auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

  camera cam;

  cam.aspectRatio = 16.0 / 9.0;
  cam.imgWidth = 400;
  cam.sampleCount = 100;
  cam.sampleDepth = 50;

  cam.verticalFOV = 20;
  cam.lookFrom = point3(0, 0, 12);
  cam.lookAt = point3(0, 0, 0);
  cam.verticalUp = vect(0, 1, 0);

  cam.defocusAngle = 0;

  cam.render(sceneObjectList(globe));
}

int main() {
  int choice;
  vector<string> functions{"bouncingSpheres", "checkedSpheres", "threeSpheres",
                           "earth"};
  for (int i = 0; i < (int)functions.size(); i++)
    clog << i + 1 << " : " << functions[i] << endl;
  clog << "Enter the Choice: ";
  cin >> choice;
  switch (choice) {
  case 1:
    bouncingSpheres();
    break;
  case 2:
    checkedSpheres();
    break;
  case 3:
    threeSpheres();
    break;
  case 4:
    earth();
    break;
  default:
    threeSpheres();
    break;
  }
}
