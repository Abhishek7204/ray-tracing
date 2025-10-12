#ifndef PERLIN_H
#define PERLIN_H

#include "rt_utility.h"
#include "vect.h"
#include <cmath>
class perlinNoise {
  static const int pointCount = 256;
  double randFloat[pointCount];
  int permX[pointCount];
  int permY[pointCount];
  int permZ[pointCount];

  static void perlinGenerater(int *p) {
    for (int i = 0; i < pointCount; i++)
      p[i] = i;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(p, p + pointCount, std::default_random_engine(seed));
  }

  static double trilinearInterpretation(double c[2][2][2], double u, double v,
                                        double w) {
    double sum = 0.0;
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        for (int k = 0; k < 2; k++)
          sum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) *
                 (k * w + (1 - k) * (1 - w)) * c[i][j][k];
    return sum;
  }

public:
  perlinNoise() {
    for (int i = 0; i < pointCount; i++)
      randFloat[i] = randomDouble();

    perlinGenerater(permX);
    perlinGenerater(permY);
    perlinGenerater(permZ);
  }

  double noise(const point3 &p) const {
    auto u = p.x() - floor(p.x());
    auto v = p.y() - floor(p.y());
    auto w = p.z() - floor(p.z());
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);

    auto i = int(floor(p.x()));
    auto j = int(floor(p.y()));
    auto k = int(floor(p.z()));
    double c[2][2][2];

    for (int di = 0; di < 2; di++)
      for (int dj = 0; dj < 2; dj++)
        for (int dk = 0; dk < 2; dk++)
          c[di][dj][dk] =
              randFloat[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^
                        permZ[(k + dk) & 255]];
    return trilinearInterpretation(c, u, v, w);
  }
};
#endif // !PERLIN_H
