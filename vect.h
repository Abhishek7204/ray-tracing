#ifndef VECT_H
#define VECT_H

#include "rt_utility.h"
#include <cmath>
#include <iostream>

using namespace std;

class vect {
  double v[3];

public:
  vect() : v{0, 0, 0} {}
  vect(double v1, double v2, double v3) : v{v1, v2, v3} {}

  double x() const { return v[0]; }
  double y() const { return v[1]; }
  double z() const { return v[2]; }
  double &operator[](int i) { return v[i]; }
  double operator[](int i) const { return v[i]; }

  vect operator-() const;
  vect operator+=(const vect &vec);
  vect operator*=(const vect &vec);
  vect operator*=(double s);
  vect operator-=(const vect &vec);
  vect operator/=(double s);

  double lenSquared() const;
  double len() const;
  bool nearZero() const;

  static vect random();
  static vect random(double lower, double upper);
};

using point3 = vect;

inline ostream &operator<<(ostream &out, const vect &vec) {
  return out << vec[0] << ' ' << vec[1] << ' ' << vec[2];
}

inline vect operator+(const vect &vec1, const vect &vec2) {
  return vect(vec1[0] + vec2[0], vec1[1] + vec2[1], vec1[2] + vec2[2]);
}

inline vect operator-(const vect &vec1, const vect &vec2) {
  return vect(vec1[0] - vec2[0], vec1[1] - vec2[1], vec1[2] - vec2[2]);
}

inline vect operator*(const vect &vec1, const vect &vec2) {
  return vect(vec1[0] * vec2[0], vec1[1] * vec2[1], vec1[2] * vec2[2]);
}

inline vect operator*(const vect &vec, double s) {
  return vect(vec[0] * s, vec[1] * s, vec[2] * s);
}

inline vect operator*(double s, const vect &vec) {
  return vect(vec[0] * s, vec[1] * s, vec[2] * s);
}

inline vect operator/(const vect &vec, double s) {
  return vect(vec[0] / s, vec[1] / s, vec[2] / s);
}

inline double dotProduct(const vect &vec1, const vect &vec2) {
  return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

inline vect crossProduct(const vect &vec1, const vect &vec2) {
  return vect(vec1[1] * vec2[2] - vec1[2] * vec2[1],
              vec1[2] * vec2[0] - vec1[0] * vec2[2],
              vec1[0] * vec2[1] - vec1[1] * vec2[0]);
}

inline vect unitVector(const vect &vec) { return vec / vec.len(); }

inline vect randomUnitVector() {
  double lenSq;
  vect r;
  do {
    r = vect::random(-1, 1);
    lenSq = r.lenSquared();
  } while (lenSq < 1e-160 || lenSq > 1);
  return unitVector(r);
}

inline vect randomOnUnitDisk() {
  double lenSq;
  vect p;
  do {
    p = vect(randomDouble(-1, 1), randomDouble(-1, 1), 0);
    lenSq = p.lenSquared();
  } while (lenSq >= 1);
  return p;
}

inline vect randomOnHemispehre(const vect &normal) {
  vect direction = randomUnitVector();
  if (dotProduct(direction, normal) < 0)
    direction = direction * -1;
  return direction;
}

inline vect reflection(const vect &normal, const vect &incident) {
  return incident - 2 * dotProduct(normal, incident) * normal;
}

inline double reflectance(double cosine, double refractriveIndex) {
  auto r0 = (1 - refractriveIndex) / (1 + refractriveIndex);
  r0 = r0 * r0;
  return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

inline vect refract(const vect &normal, const vect &incident,
                    double refractionRatio) {
  double cosTheta = fmin(dotProduct(-incident, normal), 1.0);
  double sinTheta = sqrt(1.0 - cosTheta * cosTheta);
  if (sinTheta * refractionRatio > 1.0 ||
      reflectance(cosTheta, refractionRatio) > randomDouble())
    return reflection(normal, incident);
  vect RoutPerp = refractionRatio * (incident + cosTheta * normal);
  vect RoutParell = -sqrt(abs(1 - RoutPerp.lenSquared())) * normal;
  return RoutParell + RoutPerp;
}

#endif // !VECT_H
