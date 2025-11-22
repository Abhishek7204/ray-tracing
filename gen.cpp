#include "bvh.h"
#include "camera.h"
#include "material.h"
#include "quadrilateral.h"
#include "rt_utility.h"
#include "scene_object.h"
#include "scene_object_list.h"
#include "sphere.h"
#include "volumes.h"

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
  cam.backGround = color(0.70, 0.80, 1.00);

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
  cam.backGround = color(0.70, 0.80, 1.00);

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
  cam.backGround = color(0.70, 0.80, 1.00);

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
  cam.backGround = color(0.70, 0.80, 1.00);

  cam.defocusAngle = 0;

  cam.render(sceneObjectList(globe));
}

void perlinSpheres() {
  sceneObjectList world;

  auto pertext = make_shared<noiseTexture>(4);
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                                make_shared<lambertian>(pertext)));
  world.add(make_shared<sphere>(point3(0, 2, 0), 2,
                                make_shared<lambertian>(pertext)));

  camera cam;

  cam.aspectRatio = 16.0 / 9.0;
  cam.imgWidth = 400;
  cam.sampleCount = 100;
  cam.sampleDepth = 50;

  cam.verticalFOV = 20;
  cam.lookFrom = point3(13, 2, 3);
  cam.lookAt = point3(0, 0, 0);
  cam.verticalUp = vect(0, 1, 0);
  cam.backGround = color(0.70, 0.80, 1.00);

  cam.defocusAngle = 0;

  cam.render(world);
}

void quadrilaterals() {
  sceneObjectList world;

  // Materials
  auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
  auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
  auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
  auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
  auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

  // quadrilaterals
  world.add(make_shared<quadrilateral>(point3(-3, -2, 5), vect(0, 0, -4),
                                       vect(0, 4, 0), left_red));
  world.add(make_shared<quadrilateral>(point3(-2, -2, 0), vect(4, 0, 0),
                                       vect(0, 4, 0), back_green));
  world.add(make_shared<quadrilateral>(point3(3, -2, 1), vect(0, 0, 4),
                                       vect(0, 4, 0), right_blue));
  world.add(make_shared<quadrilateral>(point3(-2, 3, 1), vect(4, 0, 0),
                                       vect(0, 0, 4), upper_orange));
  world.add(make_shared<quadrilateral>(point3(-2, -3, 5), vect(4, 0, 0),
                                       vect(0, 0, -4), lower_teal));

  camera cam;

  cam.aspectRatio = 1.0;
  cam.imgWidth = 400;
  cam.sampleCount = 100;
  cam.sampleDepth = 50;

  cam.verticalFOV = 80;
  cam.lookFrom = point3(0, 0, 9);
  cam.lookAt = point3(0, 0, 0);
  cam.verticalUp = vect(0, 1, 0);
  cam.backGround = color(0.70, 0.80, 1.00);

  cam.defocusAngle = 0;

  cam.render(world);
}

void simpleLight() {
  sceneObjectList world;

  auto pertext = make_shared<noiseTexture>(4);
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                                make_shared<lambertian>(pertext)));
  world.add(make_shared<sphere>(point3(0, 2, 0), 2,
                                make_shared<lambertian>(pertext)));

  auto difflight = make_shared<diffuseLight>(color(4, 4, 4));
  world.add(make_shared<quadrilateral>(point3(3, 1, -2), vect(2, 0, 0),
                                       vect(0, 2, 0), difflight));

  camera cam;

  cam.aspectRatio = 16.0 / 9.0;
  cam.imgWidth = 400;
  cam.sampleCount = 100;
  cam.sampleDepth = 50;
  cam.backGround = color(0, 0, 0);

  cam.verticalFOV = 20;
  cam.lookFrom = point3(26, 3, 6);
  cam.lookAt = point3(0, 2, 0);
  cam.verticalUp = vect(0, 1, 0);

  cam.defocusAngle = 0;

  cam.render(world);
}

void cornellBox() {
  sceneObjectList world;

  auto red = make_shared<lambertian>(color(.65, .05, .05));
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  auto green = make_shared<lambertian>(color(.12, .45, .15));
  auto light = make_shared<diffuseLight>(color(15, 15, 15));

  world.add(make_shared<quadrilateral>(point3(555, 0, 0), vect(0, 555, 0),
                                       vect(0, 0, 555), green));
  world.add(make_shared<quadrilateral>(point3(0, 0, 0), vect(0, 555, 0),
                                       vect(0, 0, 555), red));
  world.add(make_shared<quadrilateral>(point3(343, 554, 332), vect(-130, 0, 0),
                                       vect(0, 0, -105), light));
  world.add(make_shared<quadrilateral>(point3(0, 0, 0), vect(555, 0, 0),
                                       vect(0, 0, 555), white));
  world.add(make_shared<quadrilateral>(point3(555, 555, 555), vect(-555, 0, 0),
                                       vect(0, 0, -555), white));
  world.add(make_shared<quadrilateral>(point3(0, 0, 555), vect(555, 0, 0),
                                       vect(0, 555, 0), white));
  shared_ptr<sceneObject> box1 =
      box(point3(0, 0, 0), point3(165, 330, 165), white);
  box1 = make_shared<rotateY>(box1, 15);
  box1 = make_shared<translate>(box1, vect(265, 0, 295));
  world.add(box1);

  shared_ptr<sceneObject> box2 =
      box(point3(0, 0, 0), point3(165, 165, 165), white);
  box2 = make_shared<rotateY>(box2, -18);
  box2 = make_shared<translate>(box2, vect(130, 0, 65));
  world.add(box2);

  camera cam;

  cam.aspectRatio = 1.0;
  cam.imgWidth = 600;
  cam.sampleCount = 200;
  cam.sampleDepth = 50;
  cam.backGround = color(0, 0, 0);

  cam.verticalFOV = 40;
  cam.lookFrom = point3(278, 278, -800);
  cam.lookAt = point3(278, 278, 0);
  cam.verticalUp = vect(0, 1, 0);

  cam.defocusAngle = 0;

  cam.render(world);
}

void cornellSmoke() {
  sceneObjectList world;

  auto red = make_shared<lambertian>(color(.65, .05, .05));
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  auto green = make_shared<lambertian>(color(.12, .45, .15));
  auto light = make_shared<diffuseLight>(color(7, 7, 7));

  world.add(make_shared<quadrilateral>(point3(555, 0, 0), vect(0, 555, 0),
                                       vect(0, 0, 555), green));
  world.add(make_shared<quadrilateral>(point3(0, 0, 0), vect(0, 555, 0),
                                       vect(0, 0, 555), red));
  world.add(make_shared<quadrilateral>(point3(113, 554, 127), vect(330, 0, 0),
                                       vect(0, 0, 305), light));
  world.add(make_shared<quadrilateral>(point3(0, 555, 0), vect(555, 0, 0),
                                       vect(0, 0, 555), white));
  world.add(make_shared<quadrilateral>(point3(0, 0, 0), vect(555, 0, 0),
                                       vect(0, 0, 555), white));
  world.add(make_shared<quadrilateral>(point3(0, 0, 555), vect(555, 0, 0),
                                       vect(0, 555, 0), white));

  shared_ptr<sceneObject> box1 =
      box(point3(0, 0, 0), point3(165, 330, 165), white);
  box1 = make_shared<rotateY>(box1, 15);
  box1 = make_shared<translate>(box1, vect(265, 0, 295));

  shared_ptr<sceneObject> box2 =
      box(point3(0, 0, 0), point3(165, 165, 165), white);
  box2 = make_shared<rotateY>(box2, -18);
  box2 = make_shared<translate>(box2, vect(130, 0, 65));

  world.add(make_shared<volumes>(box1, 0.01, color(0, 0, 0)));
  world.add(make_shared<volumes>(box2, 0.01, color(1, 1, 1)));

  camera cam;

  cam.aspectRatio = 1.0;
  cam.imgWidth = 600;
  cam.sampleCount = 200;
  cam.sampleDepth = 50;
  cam.backGround = color(0, 0, 0);

  cam.verticalFOV = 40;
  cam.lookFrom = point3(278, 278, -800);
  cam.lookAt = point3(278, 278, 0);
  cam.verticalUp = vect(0, 1, 0);

  cam.defocusAngle = 0;

  cam.render(world);
}

void finalScene() {
  sceneObjectList boxes1;
  auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

  int boxes_per_side = 20;
  for (int i = 0; i < boxes_per_side; i++) {
    for (int j = 0; j < boxes_per_side; j++) {
      auto w = 100.0;
      auto x0 = -1000.0 + i * w;
      auto z0 = -1000.0 + j * w;
      auto y0 = 0.0;
      auto x1 = x0 + w;
      auto y1 = randomDouble(1, 101);
      auto z1 = z0 + w;

      boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
    }
  }

  sceneObjectList world;

  world.add(make_shared<bvh>(boxes1));

  auto light = make_shared<diffuseLight>(color(7, 7, 7));
  world.add(make_shared<quadrilateral>(point3(123, 554, 147), vect(300, 0, 0),
                                       vect(0, 0, 265), light));

  auto center1 = point3(400, 400, 200);
  auto center2 = center1 + vect(30, 0, 0);
  auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
  world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

  world.add(make_shared<sphere>(point3(260, 150, 45), 50,
                                make_shared<dielectric>(1.5)));
  world.add(make_shared<sphere>(point3(0, 150, 145), 50,
                                make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

  auto boundary = make_shared<sphere>(point3(360, 150, 145), 70,
                                      make_shared<dielectric>(1.5));
  world.add(boundary);
  world.add(make_shared<volumes>(boundary, 0.2, color(0.2, 0.4, 0.9)));
  boundary =
      make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
  world.add(make_shared<volumes>(boundary, .0001, color(1, 1, 1)));

  auto emat =
      make_shared<lambertian>(make_shared<imageTexture>("earthmap.jpg"));
  world.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
  auto pertext = make_shared<noiseTexture>(0.2);
  world.add(make_shared<sphere>(point3(220, 280, 300), 80,
                                make_shared<lambertian>(pertext)));

  sceneObjectList boxes2;
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  int ns = 1000;
  for (int j = 0; j < ns; j++) {
    boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
  }

  world.add(
      make_shared<translate>(make_shared<rotateY>(make_shared<bvh>(boxes2), 15),
                             vect(-100, 270, 395)));

  camera cam;

  cam.aspectRatio = 1.0;
  cam.imgWidth = 800;
  cam.sampleCount = 1000;
  cam.sampleDepth = 40;
  cam.backGround = color(0, 0, 0);

  cam.verticalFOV = 40;
  cam.lookFrom = point3(478, 278, -600);
  cam.lookAt = point3(278, 278, 0);
  cam.verticalUp = vect(0, 1, 0);

  cam.defocusAngle = 0;

  cam.render(world);
}

int main() {
  int choice;
  vector<void (*)()> functions{bouncingSpheres, checkedSpheres, threeSpheres,
                               perlinSpheres,   quadrilaterals, simpleLight,
                               cornellBox,      cornellSmoke,   finalScene};
  vector<string> scenes{"bouncingSpheres", "checkedSpheres", "threeSpheres",
                        "perlinSpheres",   "quadrilaterals", "simpleLight",
                        "cornellBox",      "cornellSmoke",   "finalScene"};
  for (int i = 0; i < (int)functions.size(); i++)
    clog << i + 1 << " : " << scenes[i] << endl;
  clog << "Enter the Choice: ";
  cin >> choice;
  (*functions[choice - 1])();
}
