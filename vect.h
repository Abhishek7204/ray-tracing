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
  vect operator-=(const vect &vec);
  vect operator/=(double s);

  double lenSquared() const;
  double len() const;

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

inline vect operator/(double s, const vect &vec) {
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

inline vect randomOnHemispehre(const vect &normal) {
  vect direction = randomUnitVector();
  if (dotProduct(direction, normal) < 0)
    direction = direction * -1;
  return direction;
}
#endif
